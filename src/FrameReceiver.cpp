/*
 * FrameReceiver.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameReceiver.h"

#include <stdlib.h>

namespace hdlc {

FrameReceiver::FrameReceiver(EscapingSource& source) : source(source), payloadSize(0), frameHandler(NULL), expectedSequenceNumber(0), header(0x00), crc(0xFFFF) {
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
			payload[payloadSize++] = octet;
			crc_ccitt_update(crc, octet);
			PT_WAIT_UNTIL(&pt, source.isReady());
		}

		// check crc
		if(crc == 0) {
			if(header == expectedSequenceNumber) {
				// this is an in-sequence user frame
				if(frameHandler) frameHandler->handle(header, payload, payloadSize-2);
				++expectedSequenceNumber;
			} else if(header & CONTROL_BITS) {
				// this is a control frame
				if(frameHandler) frameHandler->handle(header, NULL, 0);
			}
		}
		payloadSize = 0;
	}
	PT_END(&pt);
}

void FrameReceiver::setFrameHandler(FrameHandler* frameHandler) {
	this->frameHandler = frameHandler;
}

SequenceNumber FrameReceiver::getExpectedSequenceNumber() {
	return expectedSequenceNumber;
}

} /* namespace hdlc */
