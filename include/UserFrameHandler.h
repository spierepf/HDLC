/*
 * UserFrameHandler.h
 *
 *  Created on: Sep 28, 2015
 *      Author: peter
 */

#ifndef USERFRAMEHANDLER_H_
#define USERFRAMEHANDLER_H_

class UserFrameHandler {
public:
	UserFrameHandler() {};
	virtual ~UserFrameHandler() {};

	virtual void handle(const uint8_t*, const uint8_t) = 0;
};

#endif /* USERFRAMEHANDLER_H_ */
