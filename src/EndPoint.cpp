/*
 * EndPoint.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#include "EndPoint.h"

namespace hdlc {

EndPoint::EndPoint(EscapingSource& source, FrameReceiver& receiver, FrameHandler& handler, FrameBuffer& outgoingFrameBuffer, FrameTransmitter& transmitter, EscapingSink& sink) : source(source), receiver(receiver), handler(handler), outgoingFrameBuffer(outgoingFrameBuffer), transmitter(transmitter), sink(sink), expectedSequenceNumber(0), sendAck(false), zeroFrame(0), lastAckReceived(0) {
	// TODO Auto-generated constructor stub

}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(EndPoint::run()) {
	PT_BEGIN(&pt);
	source.schedule();
	receiver.schedule();

	if(transmitter.isReady()) {
		while(zeroFrame != lastAckReceived) {
			outgoingFrameBuffer.removeFrame();
			++zeroFrame;
		}
		if(sendAck || outgoingFrameBuffer.isEmpty()) {
			transmitter.transmit(FrameTransmitter::ACK + expectedSequenceNumber);
			sendAck = false;
		} else {
			transmitter.transmit(zeroFrame, outgoingFrameBuffer[0]);
			sendAck = true;
		}
	}

	transmitter.schedule();
	sink.schedule();
	PT_END(&pt);
}

void EndPoint::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	if((header & FrameReceiver::CONTROL_BITS) == FrameReceiver::ACK) {
		lastAckReceived = header;
	} else if(header == expectedSequenceNumber) {
		++expectedSequenceNumber;
		handler.handle(header, payload, payloadSize);
	}
}

} /* namespace hdlc */
