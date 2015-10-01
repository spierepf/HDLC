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
		State(EndPoint& endPoint) : endPoint(endPoint) {
		}

		virtual void go() {};

		virtual void handle(const uint8_t, const uint8_t*, const uint8_t) {};
	};

	class Connected : public State {
		FrameBuffer& outgoingFrameBuffer;
		SequenceNumber zeroFrame;
		SequenceNumber lastAckReceived;
		bool sendAck;
		SequenceNumber expectedSequenceNumber;

	public:
		Connected(EndPoint& endPoint, FrameBuffer& outgoingFrameBuffer) : State(endPoint), outgoingFrameBuffer(outgoingFrameBuffer), zeroFrame(0), lastAckReceived(0), sendAck(false), expectedSequenceNumber(0) {
		}

		virtual void go() {
			if(endPoint.transmitter.isReady()) {
				while(zeroFrame != lastAckReceived) {
					outgoingFrameBuffer.removeFrame();
					++zeroFrame;
				}
				if(sendAck || outgoingFrameBuffer.isEmpty()) {
					endPoint.transmitter.transmit(FrameTransmitter::ACK + expectedSequenceNumber);
					sendAck = false;
				} else {
					endPoint.transmitter.transmit(zeroFrame, outgoingFrameBuffer[0]);
					sendAck = true;
				}
			}
		}

		virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
			if((header & FrameReceiver::CONTROL_BITS) == FrameReceiver::ACK) {
				lastAckReceived = header;
			} else if(header == expectedSequenceNumber) {
				++expectedSequenceNumber;
				endPoint.handler.handle(header, payload, payloadSize);
			}
		};
	};

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

	virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
};

} /* namespace hdlc */

#endif /* ENDPOINT_H_BROKEN_ */
