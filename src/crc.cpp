/*
 * crc.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "crc.h"

namespace hdlc {

void crc_ccitt_update (uint16_t& crc, uint8_t data) {
	unsigned crc_new = (unsigned char)(crc >> 8) | (crc << 8);
	crc_new ^= data;
	crc_new ^= (unsigned char)(crc_new & 0xff) >> 4;
	crc_new ^= crc_new << 12;
	crc_new ^= (crc_new & 0xff) << 5;
	crc = crc_new;
}

} /* namespace hdlc */
