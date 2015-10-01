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

namespace hdlc {

class FrameTransmitter: public Thread {
	EscapingSink& sink;
	FrameBuffer& frameBuffer;
	uint16_t crc;
	uint8_t position;
	SequenceNumber frameZero;
	SequenceNumber ackToSend;
	SequenceNumber lastAckReceived;
	bool sendAck;

protected:
	virtual PT_THREAD(run());

public:
	enum {
		ACK = 0x40
	};

	FrameTransmitter(EscapingSink&, FrameBuffer&);
	virtual ~FrameTransmitter();

	void setAckToSend(SequenceNumber);
	void setLastAckReceived(SequenceNumber);
};

} /* namespace hdlc */

#endif /* FRAMETRANSMITTER_H_ */
