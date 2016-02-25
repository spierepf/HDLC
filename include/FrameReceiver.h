/*
 * FrameReceiver.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef FRAMERECEIVER_H_
#define FRAMERECEIVER_H_

#include <FrameHandler.h>
#include <Thread.h>

#include "EscapingSource.h"

#include "crc.h"

namespace hdlc {

class FrameReceiver : public Thread<FrameReceiver> {
	EscapingSource& source;
	uint8_t payload[64];
	uint8_t payloadSize;
	uint8_t header;
	uint16_t crc;
	FrameHandler* frameHandler;

public:
	enum {
		ACK = 0x40,
		CONTROL_BITS = 0xC0,
		SYN_DISCONNECT = 0x80,
		SYN_REQUEST = 0x81,
		SYN_RESPONSE = 0x82,
		SYN_COMPLETE = 0x83
	};

	FrameReceiver(EscapingSource&);
	~FrameReceiver();

	PT_THREAD(run());

	void setFrameHandler(FrameHandler*);
};

} /* namespace hdlc */

#endif /* FRAMERECEIVER_H_ */
