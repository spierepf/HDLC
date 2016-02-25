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

class EndPoint: public Thread<EndPoint>, public FrameHandler {
	class State : public FrameHandler {
	protected:
		EndPoint& endPoint;

	public:
		bool sendSyn;
		uint32_t idleCount;
		uint32_t timeout;

		State(EndPoint& endPoint) : endPoint(endPoint), sendSyn(false), idleCount(0) {
#ifdef AVR
			timeout = 0x000007FF;
#else
			timeout = 0x0007FFFF;
#endif
		}

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

public:
	EndPoint(EscapingSource&, FrameReceiver&, FrameHandler&, FrameBuffer&, FrameTransmitter&, EscapingSink&);
	~EndPoint();

	PT_THREAD(run());

	void connect();
	bool isConnected();

	void handle(const uint8_t, const uint8_t*, const uint8_t);
};

} /* namespace hdlc */

#endif /* ENDPOINT_H_BROKEN_ */
