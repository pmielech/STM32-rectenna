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
	MEAS,
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

typedef enum {
	SINGLE,
	MULTIPLE,
	MULTIPLE_RAND
} record_meas_t ;

typedef enum {
	CON,
	INC,
	RND
} choose_meas_val_t ;



void vGet_raw_value();
void vGet_opamp_val();
void vGain_adjustment();
void vDev_process();

#endif /* INC_MYDEVICE_DEV_ADC_H_ */
