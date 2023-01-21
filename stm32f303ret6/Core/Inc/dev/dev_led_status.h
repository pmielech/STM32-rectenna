/*
 * dev_led_status.h
 *
 *  Created on: Dec 4, 2022
 *      Author: patryk
 */

#ifndef INC_DEV_DEV_LED_STATUS_H_
#define INC_DEV_DEV_LED_STATUS_H_

#define SYS_LED_Pin GPIO_PIN_5
#define SYS_LED_GPIO_Port GPIOA

#include "stdint.h"

typedef enum {
	STARTUP,
	MEAS_READY,
	DMA_MEAS,
	DATA_PROC,
	SEND_ALL
} dev_status_t;

void vLed_indicator(dev_status_t control_state);

#endif /* INC_DEV_DEV_LED_STATUS_H_ */
