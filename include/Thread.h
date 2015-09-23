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

class Thread {
protected:
	struct pt pt;
	virtual PT_THREAD(run()) = 0;

public:
	Thread();
	virtual ~Thread();

	void schedule();
};

} /* namespace hdlc */

#endif /* THREAD_H_ */
