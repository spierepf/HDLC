/*
 * Sink.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef SINK_H_
#define SINK_H_

namespace hdlc {

template<class X>
class Sink {
protected:
	Sink() {
	}

public:
	virtual ~Sink() {
	}

	virtual bool isReady() = 0;
	virtual void write(const X&) = 0;
};

} /* namespace hdlc */

#endif /* SINK_H_ */
