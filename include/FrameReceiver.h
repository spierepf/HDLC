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

class FrameReceiver : public Thread {
	EscapingSource& source;
	uint8_t payload[64];
	uint8_t payloadSize;
	uint8_t header;
	uint16_t crc;
	FrameHandler* frameHandler;

protected:
	virtual PT_THREAD(run());

public:
	enum {
		ACK = 0x40,
		CONTROL_BITS = 0xC0
	};

	FrameReceiver(EscapingSource&);
	virtual ~FrameReceiver();

	void setFrameHandler(FrameHandler*);
};

} /* namespace hdlc */

#endif /* FRAMERECEIVER_H_ */
