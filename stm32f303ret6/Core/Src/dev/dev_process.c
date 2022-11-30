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


static uint8_t array_cnt = 0;
static uint8_t meas_complited = 0;
static uint8_t *ValArray[32];
static uint32_t testOp;
static uint32_t testRaw;
static uint8_t *rawVal;
static uint8_t *opampVal;
static adc_events_t adc_event_handler = 0;
static uint8_t *digestGenerated = 0;

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
		z1 += digestGenerated[i];
	}

	for(int i = 8; i < 16; i++){
			z2 += digestGenerated[i];
		}

	for(int i = 16; i < 24; i++){
			z3 += digestGenerated[i];
		}

	for(int i = 24; i < 32; i++){
			z4 += digestGenerated[i];
		}

	 randomGenerated = lfsr113();

}

void vMulti_meas(serial_data_t dataType) {

	switch(dataType){

	case OP_AMP:

			vGet_raw_value();
			vGain_adjustment();
			vGet_opamp_val();
			ValArray[array_cnt] = opampVal;
			//20-100 ms

			if(array_cnt == 31){
				meas_complited = 1;
				array_cnt = 0;
			}
			else{
				array_cnt++;
			}


		break;

	default:
		break;

	}


}

void vGet_raw_value() {

	//adc pA_0
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  	rawVal = (uint8_t*)HAL_ADC_GetValue(&hadc1);
  	testRaw = HAL_ADC_GetValue(&hadc1);


}

void vGet_opamp_val() {

	//opamp pA_2
	opampVal = (uint8_t*)HAL_ADC_GetValue(&hadc4);
	testOp = HAL_ADC_GetValue(&hadc4);

}

void vGenerete_digest(){
	digestGenerated = sha256_data(opampVal, sizeof(*opampVal));
}


void vGain_adjustment() {

	if(*rawVal < 256) {
		vCustom_gain(16);
	}
	if(*rawVal >= 256 && *rawVal < 512) {
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
					printf("%02x", digestGenerated[i]);
		}
		printf("\n\r");

	}
	else if (serial_data_type == RANDOM) {
		printf("R: %lu\n\r", randomGenerated);

	}

}


void vDev_process() {

	switch(adc_event_handler) {

	case MEAS:
		vMulti_meas(OP_AMP);

		if(meas_complited == 1){
			adc_event_handler = GENERATE_DIGEST;
		}

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
		adc_event_handler = MEAS;
		break;

	default:
		caseBreaker++;
		break;
	}
}
