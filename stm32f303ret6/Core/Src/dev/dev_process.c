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

#include "stm32f3xx.h"

#include "stdio.h"
#include "stdlib.h"



static char DataString[5];

static uint32_t ValArray[32];
static uint32_t meas_value = 0;
static uint32_t randomGenerated = 0;
static uint32_t opampVal = 0;


static uint8_t meas_idx = 0;
static uint8_t array_cnt = 0;
static uint8_t meas_complited = 0;
static uint8_t caseBreaker = 0;
static adc_events_t adc_event_handler = 0;

uint32_t rawVal = 0;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc4;
extern UART_HandleTypeDef huart2;

extern uint8_t Digest[32];
extern uint32_t z1;
extern uint32_t z2;
extern uint32_t z3;
extern uint32_t z4;


int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}

int iRandom(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

static int iGet_string_length(char data[]){

    int until_end_cnt = 0;

    for(int i = 0; i <= 5; i++){
        if( data[i] == '\0'){

            return until_end_cnt;
        }
        else{
            until_end_cnt ++;
        }
    }
    return 0;


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
				meas_idx = 0;
			}
			else{
				array_cnt++;
			}


		break;

	case RAW:

		vGet_raw_value();
		ValArray[array_cnt] = rawVal;
		//20-100 ms

		if(array_cnt == 31){
			meas_complited = 1;
			array_cnt = 0;
			meas_idx = 0;
		}
		else{
			array_cnt++;
		}



	default:
		break;

	}


}

void vChoose_Val(choose_meas_val_t variant){
	switch(variant){
	case CONST:
		meas_value = ValArray[meas_idx];
		break;

	case NEXT:
		meas_value = ValArray[meas_idx];
		meas_idx++;
		break;

	case RANDM:
		meas_value = ValArray[meas_idx];

		if( z1 != 0){
			srand(z1);
		}
		meas_idx = iRandom(0, 31);
		break;

	default:
		meas_value = ValArray[meas_idx];
		break;

	}

	snprintf(DataString, 5, "%ld", meas_value);

}

void vGet_raw_value() {

	//adc pA_0
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  	rawVal = HAL_ADC_GetValue(&hadc1);


}

void vGet_opamp_val() {

	//opamp pA_2
	opampVal = HAL_ADC_GetValue(&hadc4);


}

void vGenerete_digest(){

	sha256_data((uint8_t*)DataString, iGet_string_length(DataString));

}




void vSerial_port_write(serial_data_t serial_data_type) {

	if(serial_data_type == RAW) {
		printf("V: %ld\n\r", rawVal);

	}
	if(serial_data_type == OP_AMP){
		printf("O: %ld\n\r", meas_value);

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

	case MEAS:
		vMulti_meas(RAW);

		if(meas_complited == 1){
			adc_event_handler = GENERATE_DIGEST;
		}

		break;

	case GENERATE_DIGEST:
		vChoose_Val(RANDM);
		vGenerete_digest();
		adc_event_handler = GENERATE_RANDOM;
		break;

	case GENERATE_RANDOM:
		vGenerate_random();
		adc_event_handler = SEND_VALUE;
		break;

	case SEND_VALUE:

		vSerial_port_write(RAW);
		adc_event_handler = MEAS;
		break;

	default:
		caseBreaker++;
		break;
	}
}
