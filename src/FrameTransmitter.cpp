/*
 * FrameTransmitter.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "FrameTransmitter.h"

namespace hdlc {

FrameTransmitter::FrameTransmitter(EscapingSink& sink) :
		sink(sink),
		ready(true),
		header(0),
		position(0),
		crc(0xFFFF) {
	// TODO Auto-generated constructor stub

}

FrameTransmitter::~FrameTransmitter() {
	// TODO Auto-generated destructor stub
}

PT_THREAD(FrameTransmitter::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, !ready);
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.writeFlag();
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(header); crc_ccitt_update(crc, header);
		for(position = 0; position < frame.size(); position++) {
			PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(frame[position]); crc_ccitt_update(crc, frame[position]);
		}
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(crc >> 8);
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.write(crc & 0xFF);
		PT_WAIT_UNTIL(&pt, sink.isReady()); sink.writeFlag();
		ready = true;
		crc = 0xFFFF;
	}
	PT_END(&pt);
}

bool FrameTransmitter::isReady() {
	return ready;
}

void FrameTransmitter::transmit(uint8_t header, const FrameBuffer::Frame frame) {
	this->header = header;
	this->frame = frame;
	ready = false;
}

} /* namespace hdlc */
