/*
 * FrameReceiver.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameReceiver.h"

namespace hdlc {

FrameReceiver::FrameReceiver(EscapingSource& source, FrameBuffer& frameBuffer) : source(source), frameBuffer(frameBuffer), expectedSequenceNumber(0x00), header(0x00), crc(0xFFFF), lastAckReceived(0x00) {
	// TODO Auto-generated constructor stub

}

FrameReceiver::~FrameReceiver() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(FrameReceiver::run()) {
	uint8_t octet;

	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, source.isReady());
		while(source.isFlag()) {
			source.next();
			PT_WAIT_UNTIL(&pt, source.isReady());
		}

		header = source.read();
		crc = 0xFFFF;
		crc_ccitt_update(crc, header);

		source.next();
		PT_WAIT_UNTIL(&pt, source.isReady());
		while(!source.isFlag()) {
			octet = source.read();
			source.next();
			PT_WAIT_UNTIL(&pt, !frameBuffer.isFull());
			frameBuffer.put(octet);
			crc_ccitt_update(crc, octet);
			PT_WAIT_UNTIL(&pt, source.isReady());
		}

		// check crc and sequence number
		if(crc == 0 && header == expectedSequenceNumber) {
			// valid user frame
			frameBuffer.revertOctets(2);
			frameBuffer.endFrame();
			++expectedSequenceNumber;
		} else if(crc == 0 && header >> 6 == 0x01) {
			// valid ack frame
			lastAckReceived = header;
			frameBuffer.revertIncompleteFrame();
		} else {
			// framing error
			frameBuffer.revertIncompleteFrame();
		}
	}
	PT_END(&pt);
}

SequenceNumber FrameReceiver::getLastAckReceived() {
	return lastAckReceived;
}

SequenceNumber FrameReceiver::getExpectedSequenceNumber() {
	return expectedSequenceNumber;
}

} /* namespace hdlc */
