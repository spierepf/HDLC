/*
 * Source.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef SOURCE_H_
#define SOURCE_H_

namespace hdlc {

template<class X>
class Source {
protected:
	Source() {
	}

public:
	virtual ~Source() {
	}

	virtual bool isReady() = 0;
	virtual X read() = 0;
};

} /* namespace hdlc */

#endif /* SOURCE_H_ */
