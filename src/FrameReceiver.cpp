/*
 * FrameReceiver.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameReceiver.h"

#include <stdlib.h>

namespace hdlc {

FrameReceiver::FrameReceiver(EscapingSource& source) : source(source), payloadSize(0), header(0x00), crc(0xFFFF), frameHandler(NULL) {
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
			if(frameHandler) frameHandler->handle(header, payloadSize < 3 ? NULL : payload, payloadSize-2);
		}
		payloadSize = 0;
	}
	PT_END(&pt);
}

void FrameReceiver::setFrameHandler(FrameHandler* frameHandler) {
	this->frameHandler = frameHandler;
}

} /* namespace hdlc */
