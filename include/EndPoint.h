/*
 * EndPoint.h
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <Thread.h>

#include "EscapingSource.h"
#include "FrameReceiver.h"
#include "FrameTransmitter.h"
#include "EscapingSink.h"

namespace hdlc {

class EndPoint: public Thread {
	EscapingSource& source;
	FrameReceiver& receiver;
	FrameTransmitter& transmitter;
	EscapingSink& sink;

protected:
	PT_THREAD(run());

public:
	EndPoint(EscapingSource&, FrameReceiver&, FrameTransmitter&, EscapingSink&);
	virtual ~EndPoint();
};

} /* namespace hdlc */

#endif /* ENDPOINT_H_ */
