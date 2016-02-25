/*
 * Thread.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "pt.h"

namespace hdlc {

template<class Derived>
class Thread {
protected:
	struct pt pt;

public:
	Thread() { PT_INIT(&pt); }
	~Thread() {}

	void schedule() { PT_SCHEDULE(static_cast<Derived*>(this)->run()); }
};

} /* namespace hdlc */

#endif /* THREAD_H_ */
