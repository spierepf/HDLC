/*
 * EscapingSource.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef ESCAPINGSOURCE_H_
#define ESCAPINGSOURCE_H_

#include <stdint.h>

#include <Source.h>
#include <Thread.h>

namespace hdlc {

class EscapingSource: public Source<uint8_t>, public Thread<EscapingSource> {
	Source<uint8_t>& source;
	bool ready;
	uint8_t octet;
	bool flag;

public:
	enum {
		MASK=0x20,
		ESCAPE=0x7d,
		FLAG=0x7e
	};

	EscapingSource(Source<uint8_t>&);
	virtual ~EscapingSource();

	PT_THREAD(run());

	virtual bool isReady();
	virtual uint8_t read();
	bool isFlag();
	void next();
};

} /* namespace hdlc */

#endif /* ESCAPINGSOURCE_H_ */
