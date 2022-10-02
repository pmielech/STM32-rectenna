/*
 * opamp2_custom_gain.h
 *
 *  Created on: 28 wrz 2022
 *      Author: patryk
 */

#ifndef INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_
#define INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_

#include "stdint.h"

void v_adc_gain_adjustment();
void v_custom_gain(uint8_t newGain);

#endif /* INC_MYDEVICE_OPAMP2_CUSTOM_GAIN_H_ */
