/*
 * FrameTransmitter.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef FRAMETRANSMITTER_H_
#define FRAMETRANSMITTER_H_

#include <Thread.h>

#include "EscapingSink.h"
#include "FrameBuffer.h"
#include "crc.h"
#include "SequenceNumber.h"

#include <stdlib.h>

namespace hdlc {

class FrameTransmitter: public Thread<FrameTransmitter> {
	EscapingSink& sink;
	bool ready;
	uint8_t header;
	FrameBuffer::Frame frame;
	uint8_t position;
	uint16_t crc;

public:
	enum {
		ACK = 0x40,
		CONTROL_BITS = 0xC0,
		SYN_DISCONNECT = 0x80,
		SYN_REQUEST = 0x81,
		SYN_RESPONSE = 0x82,
		SYN_COMPLETE = 0x83
	};

	FrameTransmitter(EscapingSink&);
	~FrameTransmitter();

	PT_THREAD(run());

	bool isReady();
	void transmit(uint8_t, FrameBuffer::Frame = FrameBuffer::Frame());
};

} /* namespace hdlc */

#endif /* FRAMETRANSMITTER_H_ */
