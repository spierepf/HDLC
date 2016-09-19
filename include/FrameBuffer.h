/*
 * FrameBuffer.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <stdlib.h>

#include "RingBuffer.h"

namespace hdlc {

template<uint8_t SIZE, uint8_t FRAME_COUNT> // must be powers of 2
class _FrameBuffer {
	uint8_t buf[SIZE];
	uint8_t tail;
	RingBuffer<FRAME_COUNT> heads;

	static uint8_t modulo_add(uint8_t a, uint8_t b) {
		return (a + b) & (SIZE - 1);
	}

public:
	class Frame {
		const _FrameBuffer* owner;
		uint8_t head;
		uint8_t length;

		Frame(const _FrameBuffer* owner, uint8_t head, uint8_t length) : owner(owner), head(head), length(length) {
		}

	public:
		Frame() : owner(NULL), head(0), length(0) {
		}

		uint8_t size() const {
			return length;
		}

		uint8_t operator[](uint8_t i) const {
			return owner->buf[modulo_add(head, i)];
		}

		friend class _FrameBuffer<SIZE, FRAME_COUNT>;
	};

	_FrameBuffer() : tail(0) {
		heads.put(tail);
	}

	void endFrame() {
		heads.put(tail);
	}

	uint8_t size() const {
		return heads.size() - 1;
	}

	void removeFrame() {
		heads.get();
	}

	const Frame operator[](uint8_t i) const {
		return Frame(this, heads.peek(i), modulo_add(heads.peek(i + 1), -heads.peek(i)));
	}

	void put(uint8_t b) {
		buf[tail] = b;
		tail = modulo_add(tail, 1);
	}

	void revertOctets(uint8_t count) {
		tail = modulo_add(tail, -count);
	}

	void revertIncompleteFrame() {
		tail = heads.peek(size());
	}

	bool isEmpty() {
		return size() == 0;
	}

	bool isFull() {
		return (size() == (FRAME_COUNT - 1)) || (modulo_add(tail, 1) == heads.peek(0));
	}
};

typedef _FrameBuffer<128, 16> FrameBuffer;

} /* namespace hdlc */

#ifndef AVR
#include <iostream>
std::ostream &operator<<(std::ostream &os, const hdlc::FrameBuffer &frameBuffer);
#endif

#endif /* FRAMEBUFFER_H_ */
