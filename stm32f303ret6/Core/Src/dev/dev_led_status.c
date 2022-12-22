/*
 * dev_led_status.c
 *
 *  Created on: Dec 4, 2022
 *      Author: patryk
 */


#include "dev/dev_led_status.h"
#include "stm32f3xx.h"
#include "main.h"




void vLed_indicator(dev_status_t control_state){

	switch(control_state){
	case STARTUP:

		for (int i = 0; i < 5; i++) {
		    HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
		    HAL_Delay(50);
		  }

		break;
	case DATA_PROC:
		HAL_GPIO_WritePin(SYS_LED_GPIO_Port, SYS_LED_Pin, GPIO_PIN_SET);
		break;


	default:
		HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin);
		break;


	}


}
