/*
 * FrameBufferUserFrameHandler.h
 *
 *  Created on: Sep 28, 2015
 *      Author: peter
 */

#ifndef FRAMEBUFFERUSERFRAMEHANDLER_H_
#define FRAMEBUFFERUSERFRAMEHANDLER_H_

#include "FrameBuffer.h"
#include "UserFrameHandler.h"

namespace hdlc {

class FrameBufferUserFrameHandler : public UserFrameHandler {
	FrameBuffer& frameBuffer;

public:
	FrameBufferUserFrameHandler(FrameBuffer&);
	virtual ~FrameBufferUserFrameHandler();

	virtual void handle(const uint8_t*, const uint8_t);
};

} /* namespace hdlc */

#endif /* FRAMEBUFFERUSERFRAMEHANDLER_H_ */
