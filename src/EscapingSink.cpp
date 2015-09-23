/*
 * EscapingSink.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "EscapingSink.h"

namespace hdlc {

EscapingSink::EscapingSink(Sink<uint8_t>& sink) : sink(sink), ready(true), octet(0), escape(true) {
	// TODO Auto-generated constructor stub

}

EscapingSink::~EscapingSink() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(EscapingSink::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, !ready);
		if(escape && (octet == FLAG || octet == ESCAPE)) {
			PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(ESCAPE);
			octet ^= MASK;
		}
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(octet);
		ready = true;
		escape = true;
	}
	PT_END(&pt);
}

bool EscapingSink::isReady() {
	return ready;
}

void EscapingSink::write(const uint8_t& octet) {
	this->octet = octet;
	ready = false;
}

void EscapingSink::writeFlag() {
	octet = FLAG;
	escape = false;
	ready = false;
}

} /* namespace hdlc */
