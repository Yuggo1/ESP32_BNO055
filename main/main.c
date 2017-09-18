/*
 * main.c
 *
 *  Created on: 17 de set de 2017
 *      Author: yuggo
 */
#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include "driver/i2c.h"
#include "bno055.h"

extern void task_bno055_normal_mode(void *ignore);

void app_main() {
	xTaskCreate(task_bno055_normal_mode, "i2c_bno055_task", 1024 * 4,
			(void* ) 0, 10, NULL);
}


