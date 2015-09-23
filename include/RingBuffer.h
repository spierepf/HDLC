/*
 * RingBuffer.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>

namespace hdlc {

template<uint8_t SIZE> // SIZE must be a power of 2
class RingBuffer {
	uint8_t buf[SIZE];
	uint8_t count;
	uint8_t head;

	static uint8_t modulo_add(uint8_t a, uint8_t b) {
		return (a + b) & (SIZE - 1);
	}

public:
	RingBuffer() : count(0), head(0) {
	}

	bool isEmpty() const {
		return 0 == count;
	}

	bool isFull() const {
		return SIZE == count;
	}

	void put(uint8_t b) {
		buf[modulo_add(head, count)] = b;
		count++;
	}

	uint8_t get() {
		uint8_t b = buf[head];
		head = modulo_add(head, 1);
		count--;
		return b;
	}

	uint8_t peek(uint8_t position) const {
		return buf[modulo_add(head, position)];
	}

	uint8_t size() const {
		return count;
	}
};

} /* namespace hdlc */

#endif /* RINGBUFFER_H_ */
