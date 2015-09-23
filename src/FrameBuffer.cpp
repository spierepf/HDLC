/*
 * FrameBuffer.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameBuffer.h"

namespace hdlc {

std::ostream &operator<<(std::ostream &os, const FrameBuffer &frameBuffer) {
	os << "{";
	for (int i = 0; i < frameBuffer.size(); i++) {
		os << " [";
		for(int j = 0; j < frameBuffer[i].size(); j++) {
			os << std::hex << (((uint)frameBuffer[i][j] < 0x10) ? "0x0" : "0x") << (uint)frameBuffer[i][j] << ((j < frameBuffer[i].size() - 1) ? ", " : "") << std::dec;
		}
		os << ((i < frameBuffer.size() - 1) ? "]," : "]");
	}
	os << " }";
	return os;
}

} /* namespace hdlc */
