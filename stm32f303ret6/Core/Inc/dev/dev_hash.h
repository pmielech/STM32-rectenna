/*
 * dev_hash.h
 *
 *  Created on: Oct 2, 2022
 *      Author: patryk
 */

#ifndef INC_DEV_DEV_HASH_H_
#define INC_DEV_DEV_HASH_H_

#include "stdio.h"


uint32_t djb33_hash(const char* s, size_t len);

#endif /* INC_DEV_DEV_HASH_H_ */
