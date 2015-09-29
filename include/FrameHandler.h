/*
 * FrameHandler.h
 *
 *  Created on: Sep 28, 2015
 *      Author: peter
 */

#ifndef FRAMEHANDLER_H_
#define FRAMEHANDLER_H_

#include <stdint.h>

namespace hdlc {

class FrameHandler {
public:
	FrameHandler() {};
	virtual ~FrameHandler() {};

	virtual void handle(const uint8_t, const uint8_t*, const uint8_t) = 0;
};

} /* namespace hdlc */

#endif /* FRAMEHANDLER_H_ */
