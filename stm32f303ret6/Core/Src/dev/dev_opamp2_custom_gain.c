/*
 * opamp2_custom_gain.c
 *
 *  Created on: 28 wrz 2022
 *      Author: patryk
 */

#include "dev/dev_opamp2_custom_gain.h"
#include "dev/dev_process.h"

#include "stm32f3xx.h"

void Error_Handler(void);

uint32_t Gain = OPAMP_PGA_GAIN_2;
extern OPAMP_HandleTypeDef hopamp4;
extern uint32_t rawVal;


void vGain_adjustment() {

	if(rawVal < MIN_RES_RANGE) {
		Gain = (OPAMP_PGA_GAIN_16);
	}
	if(rawVal >= MIN_RES_RANGE && rawVal < MID_RES_RANGE) {
		Gain = (OPAMP_PGA_GAIN_8);
	}
	else if(rawVal >= MID_RES_RANGE && rawVal < MAX_RES_RANGE) {
		Gain = (OPAMP_PGA_GAIN_4);
	}
	else if(rawVal >= MAX_RES_RANGE) {
		Gain = (OPAMP_PGA_GAIN_2);
	}
}



void vCustom_gain() {

	hopamp4.Init.PgaGain = Gain;

	hopamp4.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
	if (HAL_OPAMP_Init(&hopamp4) != HAL_OK)
	{
	Error_Handler();
	}

}
