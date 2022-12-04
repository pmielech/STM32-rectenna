/*
 * dev_led_status.c
 *
 *  Created on: Dec 4, 2022
 *      Author: patryk
 */


#include "dev/dev_led_status.h"
#include "stm32f3xx.h"


void vLed_indicator(led_status_t control_state){

	switch(control_state){
	case STARTUP:

		for (int i = 0; i < 5; i++) {
		    HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
		    HAL_Delay(50);
		  }

		break;

	case WORK_PHASE:
		HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
		break;

	default:

		break;


	}


}
