/*
 * EndPoint.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#include "EndPoint.h"

namespace hdlc {

EndPoint::EndPoint(EscapingSource& source, FrameReceiver& receiver, FrameHandler& handler, FrameBuffer& outgoingFrameBuffer, FrameTransmitter& transmitter, EscapingSink& sink) :
		disconnected(*this),
		syncRequestSent(*this),
		syncResponseSent(*this),
		connected(*this, outgoingFrameBuffer),
		state(NULL),
		source(source),
		receiver(receiver),
		handler(handler),
		transmitter(transmitter),
		sink(sink) {
	enterState(&disconnected);
}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

void EndPoint::enterState(State* newState) {
	newState->onEntry();
	state = newState;
}

PT_THREAD(EndPoint::run()) {
	PT_BEGIN(&pt);
	source.schedule();
	receiver.schedule();

	state->go();

	transmitter.schedule();
	sink.schedule();
	PT_END(&pt);
}

void EndPoint::connect() {
	state->connect();
}

bool EndPoint::isConnected() {
	return state == &connected;
}

void EndPoint::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	state->handle(header, payload, payloadSize);
}

/*****************************************************************************************************/

EndPoint::Disconnected::Disconnected(EndPoint& endPoint) : State(endPoint) {
}

void EndPoint::Disconnected::onEntry() {
}

void EndPoint::Disconnected::connect() {
	endPoint.enterState(&endPoint.syncRequestSent);
}

void EndPoint::Disconnected::go() {
	if(sendSyn && endPoint.transmitter.isReady()) {
		endPoint.transmitter.transmit(FrameTransmitter::SYN_DISCONNECT);
		sendSyn = false;
	}
}

void EndPoint::Disconnected::handle(const uint8_t header, const uint8_t*, const uint8_t) {
	switch(header) {
	case FrameReceiver::SYN_DISCONNECT:
		break;

	case FrameReceiver::SYN_REQUEST:
		endPoint.enterState(&endPoint.syncResponseSent);
		break;

	default:
		sendSyn = true;
		break;
	}
}

/*****************************************************************************************************/

EndPoint::SyncRequestSent::SyncRequestSent(EndPoint& endPoint) : State(endPoint) {
}

void EndPoint::SyncRequestSent::onEntry() {
	sendSyn = true;
}

void EndPoint::SyncRequestSent::connect() {
}

void EndPoint::SyncRequestSent::go() {
	if(sendSyn && endPoint.transmitter.isReady()) {
		endPoint.transmitter.transmit(FrameTransmitter::SYN_REQUEST);
		sendSyn = false;
	}
}

void EndPoint::SyncRequestSent::handle(const uint8_t header, const uint8_t*, const uint8_t) {
	switch(header) {
	case FrameReceiver::SYN_DISCONNECT:
		endPoint.enterState(&endPoint.disconnected);
		break;

	case FrameReceiver::SYN_REQUEST:
		endPoint.enterState(&endPoint.syncResponseSent);
		break;

	case FrameReceiver::SYN_RESPONSE:
		endPoint.connected.sendSyn = true;
		endPoint.enterState(&endPoint.connected);
		break;

	default:
		sendSyn = true;
		break;
	}
}

/*****************************************************************************************************/

EndPoint::SyncResponseSent::SyncResponseSent(EndPoint& endPoint) : State(endPoint) {
}

void EndPoint::SyncResponseSent::onEntry() {
	sendSyn = true;
}

void EndPoint::SyncResponseSent::connect() {
	endPoint.enterState(&endPoint.syncRequestSent);
}

void EndPoint::SyncResponseSent::go() {
	if(sendSyn && endPoint.transmitter.isReady()) {
		endPoint.transmitter.transmit(FrameTransmitter::SYN_RESPONSE);
		sendSyn = false;
	}
}

void EndPoint::SyncResponseSent::handle(const uint8_t header, const uint8_t*, const uint8_t) {
	switch(header) {
	case FrameReceiver::SYN_DISCONNECT:
		endPoint.enterState(&endPoint.disconnected);
		break;

	case FrameReceiver::SYN_REQUEST:
		break;

	case FrameReceiver::SYN_RESPONSE:
		endPoint.connected.sendSyn = true;
		endPoint.enterState(&endPoint.connected);
		break;

	case FrameReceiver::SYN_COMPLETE:
		endPoint.enterState(&endPoint.connected);
		break;

	default:
		sendSyn = true;
		break;
	}
}

/*****************************************************************************************************/

EndPoint::Connected::Connected(EndPoint& endPoint, FrameBuffer& outgoingFrameBuffer) : State(endPoint), outgoingFrameBuffer(outgoingFrameBuffer), zeroFrame(0), lastAckReceived(0), sendAck(false), sendUserFrame(true), idleCount(0), expectedSequenceNumber(0) {
}

void EndPoint::Connected::onEntry() {
	zeroFrame = 0;
	lastAckReceived = 0;
	sendAck = false;
	sendUserFrame = true;
	expectedSequenceNumber = false;
	while(!outgoingFrameBuffer.isEmpty()) outgoingFrameBuffer.removeFrame();
}

void EndPoint::Connected::connect() {
	endPoint.enterState(&endPoint.syncRequestSent);
}

void EndPoint::Connected::go()  {
	if(endPoint.transmitter.isReady()) {
		while(zeroFrame != lastAckReceived) {
			outgoingFrameBuffer.removeFrame();
			++zeroFrame;
		}
		if(sendSyn) {
			endPoint.transmitter.transmit(FrameTransmitter::SYN_COMPLETE);
			sendSyn = false;
			idleCount = 0;
		} else if(sendAck) {
			endPoint.transmitter.transmit(FrameTransmitter::ACK + expectedSequenceNumber);
			sendAck = false;
			idleCount = 0;
		} else if(!outgoingFrameBuffer.isEmpty()) {
			if(sendUserFrame)	{
				endPoint.transmitter.transmit(zeroFrame, outgoingFrameBuffer[0]);
				sendUserFrame = false;
				idleCount = 0;
			} else {
				idleCount++;
#ifdef AVR
				if(idleCount > 0x00000FFF) sendUserFrame = true;
#else
				if(idleCount > 0x0007FFFF) sendUserFrame = true;
#endif
			}
		}
	}
}

void EndPoint::Connected::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	switch(header) {
	case FrameReceiver::SYN_DISCONNECT:
		endPoint.enterState(&endPoint.disconnected);
		break;

	case FrameReceiver::SYN_REQUEST:
		endPoint.enterState(&endPoint.syncResponseSent);
		break;

	case FrameReceiver::SYN_RESPONSE:
		sendSyn = true;
		break;

	case FrameReceiver::SYN_COMPLETE:
		break;

	default:
		if((header & FrameReceiver::CONTROL_BITS) == FrameReceiver::ACK) {
			lastAckReceived = header;
			sendUserFrame = true;
		} else {
			sendAck = true;
			if(header == expectedSequenceNumber) {
				++expectedSequenceNumber;
				endPoint.handler.handle(header, payload, payloadSize);
			}
		}
		break;
	}
}

} /* namespace hdlc */
