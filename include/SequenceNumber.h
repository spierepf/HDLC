/*
 * SequenceNumber.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef SEQUENCENUMBER_H_
#define SEQUENCENUMBER_H_

#include <stdint.h>

namespace hdlc {

class SequenceNumber {
	uint8_t value:6;

public:
	SequenceNumber(int value = 0);
	virtual ~SequenceNumber();

	operator int();
	SequenceNumber& operator++();
};

} /* namespace hdlc */

#endif /* SEQUENCENUMBER_H_ */
