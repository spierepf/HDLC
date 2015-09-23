/*
 * EndPoint.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: peter
 */

#include "EndPoint.h"

namespace hdlc {

EndPoint::EndPoint(EscapingSource& source, FrameReceiver& receiver, FrameTransmitter& transmitter, EscapingSink& sink) : source(source), receiver(receiver), transmitter(transmitter), sink(sink) {
	// TODO Auto-generated constructor stub

}

EndPoint::~EndPoint() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(EndPoint::run()) {
	PT_BEGIN(&pt);
	source.schedule();
	receiver.schedule();

	transmitter.setLastAckReceived(receiver.getLastAckReceived());
	transmitter.setAckToSend(receiver.getExpectedSequenceNumber());

	transmitter.schedule();
	sink.schedule();
	PT_END(&pt);
}

} /* namespace hdlc */
