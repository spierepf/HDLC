/*
 * SequenceNumber.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "SequenceNumber.h"

namespace hdlc {

SequenceNumber::SequenceNumber(int value) : value(value) {
	// TODO Auto-generated constructor stub

}

SequenceNumber::~SequenceNumber() {
	// TODO Auto-generated destructor stub
}

SequenceNumber::operator int() {
	return value;
}

SequenceNumber& SequenceNumber::operator ++() {
	value++;
	return *this;
}

} /* namespace hdlc */
