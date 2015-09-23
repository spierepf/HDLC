/*
 * crc.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

namespace hdlc {

void crc_ccitt_update (uint16_t& crc, uint8_t data);

} /* namespace hdlc */

#endif /* CRC_H_ */
