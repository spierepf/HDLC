/*
 * EndPoint.h
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_BROKEN_
#define ENDPOINT_H_BROKEN_

#include <Thread.h>

#include "EscapingSource.h"
#include "FrameReceiver.h"
#include "FrameTransmitter.h"
#include "EscapingSink.h"
#include "FrameHandler.h"

namespace hdlc {

class EndPoint: public Thread, public FrameHandler {
	class State : public FrameHandler {
	protected:
		EndPoint& endPoint;

	public:
		State(EndPoint& endPoint) : endPoint(endPoint) {}

		virtual void connect() {}
		virtual void go() {}
		virtual void handle(const uint8_t, const uint8_t*, const uint8_t) {}
	};

	class Disconnected : public State {
	public:
		Disconnected(EndPoint&);

		virtual void connect();
	};

	class Connected : public State {
		FrameBuffer& outgoingFrameBuffer;
		SequenceNumber zeroFrame;
		SequenceNumber lastAckReceived;
		bool sendAck;
		SequenceNumber expectedSequenceNumber;

	public:
		Connected(EndPoint&, FrameBuffer&);

		virtual void go();
		virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize);
	};

	Disconnected disconnected;
	Connected connected;
	State* state;

	EscapingSource& source;
	FrameReceiver& receiver;
	FrameHandler& handler;
	FrameTransmitter& transmitter;
	EscapingSink& sink;

protected:
	PT_THREAD(run());

public:
	EndPoint(EscapingSource&, FrameReceiver&, FrameHandler&, FrameBuffer&, FrameTransmitter&, EscapingSink&);
	virtual ~EndPoint();

	void connect();
	virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
};

} /* namespace hdlc */

#endif /* ENDPOINT_H_BROKEN_ */
