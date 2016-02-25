/*
 * EscapingSink.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef ESCAPINGSINK_H_
#define ESCAPINGSINK_H_

#include <stdint.h>

#include <Sink.h>
#include <Thread.h>

namespace hdlc {

class EscapingSink: public Sink<uint8_t>, public Thread<EscapingSink> {
	Sink<uint8_t>& sink;
	bool ready;
	uint8_t octet;
	bool escape;

public:
	enum {
		MASK=0x20,
		ESCAPE=0x7d,
		FLAG=0x7e
	};

	EscapingSink(Sink<uint8_t>&);
	virtual ~EscapingSink();

	PT_THREAD(run());

	virtual bool isReady();
	virtual void write(const uint8_t&);
	void writeFlag();
};

} /* namespace hdlc */

#endif /* ESCAPINGSINK_H_ */
