/*
 * dev_adc.c
 *
 *  Created on: Oct 2, 2022
 *      Author: patryk
 */

#include <dev/dev_process.h>
#include "dev/dev_opamp2_custom_gain.h"
#include "dev/sha256.h"

#include "dev/lfsr113.h"
#include "dev/lfsr88.h"

#include "stm32f3xx.h"

#include "stdio.h"

static uint8_t *rawVal;
static uint8_t *opampVal;
static adc_events_t adc_event_handler = 0;
static uint8_t *Digest = 0;

static uint32_t randomGenerated = 0;
static uint8_t caseBreaker = 0;


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc4;
extern UART_HandleTypeDef huart2;

extern uint32_t z1;
extern uint32_t z2;
extern uint32_t z3;
extern uint32_t z4;


int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}


void vGenerate_random() {

	for(int i = 0; i < 8; i++){
		z1 += Digest[i];
	}

	for(int i = 8; i < 16; i++){
			z2 += Digest[i];
		}

	for(int i = 16; i < 24; i++){
			z3 += Digest[i];
		}

	for(int i = 24; i < 32; i++){
			z4 += Digest[i];
		}

	 randomGenerated = lfsr113();

}


void vGet_raw_value() {

	//adc pA_0
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  	rawVal = (uint8_t*)HAL_ADC_GetValue(&hadc1);

}

void vGet_opamp_val() {

	//opamp pA_2
	opampVal = (uint8_t*)HAL_ADC_GetValue(&hadc4);

}

void vGenerete_digest(){
	Digest = sha256_data(opampVal, sizeof(*opampVal));
}


void vGain_adjustment() {

	if(*rawVal < 256) {
		vCustom_gain(16);
	}
	else if(*rawVal >= 256 && *rawVal < 512) {
		vCustom_gain(8);
	}
	else if(*rawVal >= 512 && *rawVal < 1024) {
		vCustom_gain(4);
	}
	else if(*rawVal >= 1024) {
		vCustom_gain(2);
	}
}


void vSerial_port_write(serial_data_t serial_data_type) {
	if(serial_data_type == RAW) {
		printf("V: %p\n\r", rawVal);

	}
	if(serial_data_type == OP_AMP){
		printf("O: %p\n\r", opampVal);

	}

	if(serial_data_type == DIGEST) {

		printf("D: ");
		for(int i  = 0; i < 32; i++)
		{
					printf("%02x", Digest[i]);
		}
		printf("\n\r");

	}
	else if (serial_data_type == RANDOM) {
		printf("R: %lu\n\r", randomGenerated);

	}

}


void vDev_process() {

	switch(adc_event_handler) {

	case GET_RAW_VALUE:
		vGet_raw_value();
		adc_event_handler = SET_GAIN_VALUE;
		break;

	case SET_GAIN_VALUE:
		vGain_adjustment();
		adc_event_handler = GET_OPAMP_VALUE;
		break;

	case GET_OPAMP_VALUE:
		vGet_opamp_val();
		adc_event_handler = GENERATE_DIGEST;
		break;

	case GENERATE_DIGEST:
		vGenerete_digest();
		adc_event_handler = GENERATE_RANDOM;
		break;

	case GENERATE_RANDOM:
		vGenerate_random();
		adc_event_handler = SEND_VALUE;
		break;

	case SEND_VALUE:

		vSerial_port_write(RANDOM);
		adc_event_handler = GET_RAW_VALUE;
		break;

	default:
		caseBreaker++;
		break;
	}
}
