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
		connected(*this, outgoingFrameBuffer),
		state(&disconnected),
		source(source),
		receiver(receiver),
		handler(handler),
		transmitter(transmitter),
		sink(sink) {
	// TODO Auto-generated constructor stub

}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
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

void EndPoint::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	state->handle(header, payload, payloadSize);
}

EndPoint::Disconnected::Disconnected(EndPoint& endPoint) : State(endPoint) {
}

void EndPoint::Disconnected::connect() {
	endPoint.state = &(endPoint.connected);
}

EndPoint::Connected::Connected(EndPoint& endPoint, FrameBuffer& outgoingFrameBuffer) : State(endPoint), outgoingFrameBuffer(outgoingFrameBuffer), zeroFrame(0), lastAckReceived(0), sendAck(false), expectedSequenceNumber(0) {
}

void EndPoint::Connected::go()  {
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

void EndPoint::Connected::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	if((header & FrameReceiver::CONTROL_BITS) == FrameReceiver::ACK) {
		lastAckReceived = header;
	} else if(header == expectedSequenceNumber) {
		++expectedSequenceNumber;
		endPoint.handler.handle(header, payload, payloadSize);
	}
}

} /* namespace hdlc */
