/*
 * dev_adc.c
 *
 *  Created on: Oct 2, 2022
 *      Author: patryk
 */

#include "dev/dev_adc.h"
#include "dev/dev_opamp2_custom_gain.h"
#include "stm32f3xx.h"

#include "stdio.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart2;

static uint32_t defaultVal = 0;
static uint32_t opampVal = 0;
static adc_events_t adc_event_handler = 0;


int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}

void v_get_default_value() {

	//adc pA_0
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  	defaultVal = HAL_ADC_GetValue(&hadc1);
  	//float defaultVolt = defaultVal * REF_VALU / BIT_RESS;
}

void v_get_opamp_val() {

	//opamp pA_7
	opampVal = HAL_ADC_GetValue(&hadc2);
	//float opampVolt =   opampVal * REF_VALU / BIT_RESS;

}

void v_adc_gain_adjustment() {

	if(defaultVal < 256) {
		v_custom_gain(16);
	}
	else if(defaultVal >= 256 && defaultVal < 512) {
		v_custom_gain(8);
	}
	else if(defaultVal >= 512 && defaultVal < 1024) {
		v_custom_gain(4);
	}
	else if(defaultVal >= 1024) {
		v_custom_gain(2);
	}
}

void v_send_to_serial_port(serial_data_t serial_data_type) {
	if(serial_data_type == WRITE_DEFAULT) {
		printf("D: %lu\n\r", defaultVal);
	}
	else if (serial_data_type == WRITE_OPAMP) {
		printf("O: %lu\n\r", opampVal);
	}

}

void v_adc_process() {

	switch(adc_event_handler) {

	case GET_DEFAULT_VALUE:
		v_get_default_value();
		adc_event_handler = SET_GAIN_VALUE;
		break;

	case SET_GAIN_VALUE:
		v_adc_gain_adjustment();
		adc_event_handler = GET_OPAMP_VALUE;
		break;

	case GET_OPAMP_VALUE:
		v_get_opamp_val();
		adc_event_handler = SEND_VALUE;
		break;

	case SEND_VALUE:
		v_send_to_serial_port(WRITE_DEFAULT);
		v_send_to_serial_port(WRITE_OPAMP);
		adc_event_handler = GET_DEFAULT_VALUE;
		break;
	}
}
