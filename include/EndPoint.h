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
		bool sendSyn;

		State(EndPoint& endPoint) : endPoint(endPoint), sendSyn(false) {}

		virtual void onEntry() {}
		virtual void connect() {}
		virtual void go() {}
		virtual void handle(const uint8_t, const uint8_t*, const uint8_t) {}
	};

	class Disconnected : public State {

	public:
		Disconnected(EndPoint&);

		virtual void onEntry();
		virtual void connect();
		virtual void go();
		virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
	};

	class SyncRequestSent : public State {
	public:
		SyncRequestSent(EndPoint&);

		virtual void onEntry();
		virtual void connect();
		virtual void go();
		virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
	};

	class SyncResponseSent : public State {
	public:
		SyncResponseSent(EndPoint&);

		virtual void onEntry();
		virtual void connect();
		virtual void go();
		virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
	};

	class Connected : public State {
		FrameBuffer& outgoingFrameBuffer;
		SequenceNumber zeroFrame;
		SequenceNumber lastAckReceived;
		bool sendAck;
		bool sendUserFrame;
		uint32_t idleCount;
		SequenceNumber expectedSequenceNumber;

	public:
		Connected(EndPoint&, FrameBuffer&);

		virtual void onEntry();
		virtual void connect();
		virtual void go();
		virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize);
	};

	Disconnected disconnected;
	SyncRequestSent syncRequestSent;
	SyncResponseSent syncResponseSent;
	Connected connected;
	State* state;

	EscapingSource& source;
	FrameReceiver& receiver;
	FrameHandler& handler;
	FrameTransmitter& transmitter;
	EscapingSink& sink;

	void enterState(State*);

protected:
	PT_THREAD(run());

public:
	EndPoint(EscapingSource&, FrameReceiver&, FrameHandler&, FrameBuffer&, FrameTransmitter&, EscapingSink&);
	virtual ~EndPoint();

	void connect();
	bool isConnected();

	virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
};

} /* namespace hdlc */

#endif /* ENDPOINT_H_BROKEN_ */
