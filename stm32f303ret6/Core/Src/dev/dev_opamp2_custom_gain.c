/*
 * opamp2_custom_gain.c
 *
 *  Created on: 28 wrz 2022
 *      Author: patryk
 */

#include "dev/dev_opamp2_custom_gain.h"
#include "stm32f3xx.h"

void Error_Handler(void);
extern OPAMP_HandleTypeDef hopamp4;

void vCustom_gain(uint8_t newGain) {

	switch(newGain) {

	case 2:
		hopamp4.Init.PgaGain = OPAMP_PGA_GAIN_2;
		//TODO: check assert_param
		//assert_param(IS_OPAMP_PGA_GAIN(hopamp->Init.PgaGain));
		break;

	case 4:
		hopamp4.Init.PgaGain = OPAMP_PGA_GAIN_4;
		break;

	case 8:
		hopamp4.Init.PgaGain = OPAMP_PGA_GAIN_8;
		break;

	case 16:
		hopamp4.Init.PgaGain = OPAMP_PGA_GAIN_16;
		break;

	default:
		hopamp4.Init.PgaGain = OPAMP_PGA_GAIN_2;
		break;
	}

	hopamp4.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
	if (HAL_OPAMP_Init(&hopamp4) != HAL_OK)
	{
	Error_Handler();
	}

}
