/*
 * FrameBufferUserFrameHandler.cpp
 *
 *  Created on: Sep 28, 2015
 *      Author: peter
 */

#include "FrameBufferUserFrameHandler.h"

namespace hdlc {

FrameBufferUserFrameHandler::FrameBufferUserFrameHandler(FrameBuffer& frameBuffer) : frameBuffer(frameBuffer) {
	// TODO Auto-generated constructor stub

}

FrameBufferUserFrameHandler::~FrameBufferUserFrameHandler() {
	// TODO Auto-generated destructor stub
}

void FrameBufferUserFrameHandler::handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
	for(int i = 0; i < payloadSize; i++) {
		frameBuffer.put(payload[i]);
	}
	frameBuffer.endFrame();
}

} /* namespace hdlc */
