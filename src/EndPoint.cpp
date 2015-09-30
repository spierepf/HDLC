/*
 * EndPoint.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#include "EndPoint.h"

namespace hdlc {

EndPoint::EndPoint(EscapingSource& source, FrameReceiver& receiver, FrameHandler& handler, FrameBuffer& outgoingFrameBuffer, FrameTransmitter& transmitter, EscapingSink& sink) : source(source), receiver(receiver), handler(handler), outgoingFrameBuffer(outgoingFrameBuffer), transmitter(transmitter), sink(sink), expectedSequenceNumber(0) {
	// TODO Auto-generated constructor stub

}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(EndPoint::run()) {
	PT_BEGIN(&pt);
	source.schedule();
	receiver.schedule();
	transmitter.schedule();
	sink.schedule();
	PT_END(&pt);
}

void EndPoint::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	if((header & FrameReceiver::CONTROL_BITS) == FrameReceiver::ACK) {
		transmitter.setLastAckReceived(header);
	} else if(header == expectedSequenceNumber) {
		transmitter.setAckToSend(++expectedSequenceNumber);
		handler.handle(header, payload, payloadSize);
	}
}

} /* namespace hdlc */
