/*
 * Thread.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "Thread.h"

namespace hdlc {

Thread::Thread() {
	PT_INIT(&pt);
}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::schedule() {
	PT_SCHEDULE(run());
}

} /* namespace hdlc */
