/*
 * EscapingSource.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "EscapingSource.h"

namespace hdlc {

EscapingSource::EscapingSource(Source<uint8_t>& source) : source(source), ready(false), octet(0), flag(false) {
	// TODO Auto-generated constructor stub

}

EscapingSource::~EscapingSource() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(EscapingSource::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, !ready);
		PT_WAIT_UNTIL(&pt, source.isReady());
		octet = source.read();
		flag = false;
		if(FLAG == octet) {
			flag = true;
		} else if(ESCAPE == octet) {
			PT_WAIT_UNTIL(&pt, source.isReady()); octet = source.read() ^ MASK;
		}
		ready = true;
	}
	PT_END(&pt);
}

bool EscapingSource::isReady() {
	return ready;
}

uint8_t EscapingSource::read() {
	return octet;
}

bool EscapingSource::isFlag() {
	return flag;
}

void EscapingSource::next() {
	ready = false;
}

} /* namespace hdlc */
