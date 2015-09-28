/*
 * FrameReceiver.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef FRAMERECEIVER_H_
#define FRAMERECEIVER_H_

#include <Thread.h>

#include "EscapingSource.h"
#include "UserFrameHandler.h"
#include "SequenceNumber.h"

#include "crc.h"

namespace hdlc {

class FrameReceiver : public Thread {
	EscapingSource& source;
	uint8_t payload[64];
	uint8_t payloadSize;
	UserFrameHandler* userFrameHandler;
	SequenceNumber expectedSequenceNumber;
	uint8_t header;
	uint16_t crc;
	SequenceNumber lastAckReceived;

protected:
	virtual PT_THREAD(run());

public:
	enum {
		ACK = 0x40
	};

	FrameReceiver(EscapingSource&, UserFrameHandler*);
	virtual ~FrameReceiver();

	SequenceNumber getLastAckReceived();
	SequenceNumber getExpectedSequenceNumber();
};

} /* namespace hdlc */

#endif /* FRAMERECEIVER_H_ */