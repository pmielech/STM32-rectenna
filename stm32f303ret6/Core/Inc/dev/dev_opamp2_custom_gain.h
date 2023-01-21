/*
 * opamp2_custom_gain.h
 *
 *  Created on: 28 wrz 2022
 *      Author: patryk
 */

#ifndef INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_
#define INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_

#define MIN_RES_RANGE 256.0f
#define MID_RES_RANGE 512.0f
#define MAX_RES_RANGE 1024.0f

#include "stdint.h"

void vGain_adjustment();

void vCustom_gain();

#endif /* INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_ */
