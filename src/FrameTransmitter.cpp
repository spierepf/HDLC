/*
 * FrameTransmitter.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameTransmitter.h"

namespace hdlc {

FrameTransmitter::FrameTransmitter(EscapingSink& sink, FrameBuffer& frameBuffer) : sink(sink), frameBuffer(frameBuffer), crc(0xFFFF), position(0), frameZero(0x00), ackToSend(0x00), lastAckReceived(0x00), sendAck(false) {
	// TODO Auto-generated constructor stub

}

FrameTransmitter::~FrameTransmitter() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(FrameTransmitter::run()) {
	uint8_t octet = 0x00;

	PT_BEGIN(&pt);
	PT_WAIT_UNTIL(&pt, sink.isReady()); sink.writeFlag();
	for(;;) {
		while(frameZero != lastAckReceived) {
			frameBuffer.removeFrame();
			++frameZero;
		}
		crc = 0xFFFF;
		if(frameBuffer.size() > 0 && !sendAck) {
			PT_WAIT_UNTIL(&pt, sink.isReady());
			octet = frameZero;
			sink.write(octet);
			crc_ccitt_update(crc, octet);
			for(position = 0; position < frameBuffer[0].size(); position++) {
				PT_WAIT_UNTIL(&pt, sink.isReady());
				octet = frameBuffer[0][position];
				sink.write(octet);
				crc_ccitt_update(crc, octet);
			}
			sendAck = true;
		} else {
			PT_WAIT_UNTIL(&pt, sink.isReady());
			octet = ACK + ackToSend;
			sink.write(octet);
			crc_ccitt_update(crc, octet);
			sendAck = false;
		}
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(crc >> 8);
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(crc & 0xFF);
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.writeFlag();
	}
	PT_END(&pt);
}

SequenceNumber FrameTransmitter::getFrameZeroSequenceNumber() {
	return frameZero;
}

void FrameTransmitter::setAckToSend(SequenceNumber sequenceNumber) {
	ackToSend = sequenceNumber;
}

void FrameTransmitter::setLastAckReceived(SequenceNumber sequenceNumber) {
	lastAckReceived = sequenceNumber;
}

} /* namespace hdlc */
