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

class FrameTransmitter: public Thread {
	EscapingSink& sink;
	bool ready;
	uint8_t header;
	FrameBuffer::Frame frame;
	uint8_t position;
	uint16_t crc;

protected:
	virtual PT_THREAD(run());

public:
	enum {
		ACK = 0x40
	};

	FrameTransmitter(EscapingSink&);
	virtual ~FrameTransmitter();

	bool isReady();
	void transmit(uint8_t, FrameBuffer::Frame = FrameBuffer::Frame());
};

} /* namespace hdlc */

#endif /* FRAMETRANSMITTER_H_ */
