/*
 * dev_adc.h
 *
 *  Created on: Oct 2, 2022
 *      Author: patryk
 */

#ifndef INC_MYDEVICE_DEV_ADC_H_
#define INC_MYDEVICE_DEV_ADC_H_

#define BIT_RESS  4096.0f
#define REF_VALU  3.30f

#include "stdint.h"

typedef enum {
	GET_RAW_VALUE,
	SET_GAIN_VALUE,
	GET_OPAMP_VALUE,
	GENERATE_DIGEST,
	GENERATE_RANDOM,
	SEND_VALUE
} adc_events_t ;

typedef enum {
	RAW,
	OP_AMP,
	DIGEST,
	RANDOM
} serial_data_t ;


void v_get_raw_value();
void v_get_opamp_val();
void v_adc_gain_adjustment();
void v_dev_process();

#endif /* INC_MYDEVICE_DEV_ADC_H_ */
