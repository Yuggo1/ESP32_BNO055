/* i2c - Example

 For other examples please check:
 https://github.com/espressif/esp-idf/tree/master/examples

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include "driver/i2c.h"
#include "bno055.h"

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_MASTER_ACK 0
#define I2C_MASTER_NACK 1

void i2c_master_init() {
	i2c_config_t i2c_config = { .mode = I2C_MODE_MASTER, .sda_io_num = SDA_PIN,
			.scl_io_num = SCL_PIN, .sda_pullup_en = GPIO_PULLUP_ENABLE,
			.scl_pullup_en = GPIO_PULLUP_ENABLE, .master.clk_speed = 400000 };
	i2c_param_config(I2C_NUM_0, &i2c_config);
	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
	u8 iError = BNO055_INIT_VALUE;
	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, 1);

	i2c_master_write_byte(cmd, reg_addr, 1);
	i2c_master_write(cmd, reg_data, cnt, 1);
	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		iError = BNO055_SUCCESS;
	} else {
		iError = BNO055_ERROR;
	}
	i2c_cmd_link_delete(cmd);

	return (s8) iError;
}

s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
	u8 iError = BNO055_INIT_VALUE;
	esp_err_t espRc;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE, 1);
	i2c_master_write_byte(cmd, reg_addr, 1);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, 1);

	if (cnt > 1) {
		i2c_master_read(cmd, reg_data, cnt - 1, I2C_MASTER_ACK);
	}
	i2c_master_read_byte(cmd, reg_data + cnt - 1, I2C_MASTER_NACK);
	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		iError = BNO055_SUCCESS;
	} else {
		iError = BNO055_ERROR;
	}

	i2c_cmd_link_delete(cmd);

	return (s8) iError;
}

void BNO055_delay_msek(u32 msek) {
	vTaskDelay(msek / portTICK_PERIOD_MS);
}

void task_bno055_normal_mode(void *ignore) {
	i2c_master_init();
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	struct bno055_t myBNO = { .bus_write = BNO055_I2C_bus_write, .bus_read =
			BNO055_I2C_bus_read, .dev_addr = BNO055_I2C_ADDR1, .delay_msec =
			BNO055_delay_msek };
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	bno055_init(&myBNO);
	bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
	struct bno055_euler_float_t eulerData;
	bno055_convert_float_euler_hpr_deg(&eulerData);
	unsigned char accel_calib_status = 0;
	unsigned char gyro_calib_status = 0;
	unsigned char mag_calib_status = 0;
	unsigned char sys_calib_status = 0;

	bno055_get_accel_calib_stat(&accel_calib_status);
	bno055_get_mag_calib_stat(&mag_calib_status);
	bno055_get_gyro_calib_stat(&gyro_calib_status);
	bno055_get_sys_calib_stat(&sys_calib_status);
	s16 heading;
	s16 pitch;
	s16 roll;
	/*double yy;
	double roll;
	double pitch;*/
	float rollDeg;
	float pitchDeg;
	float yawDeg;
	while (1) {
		if(bno055_read_euler_h(&heading)==BNO055_SUCCESS&&bno055_read_euler_p(&pitch)==BNO055_SUCCESS&&bno055_read_euler_r(&roll)==BNO055_SUCCESS){
		 	rollDeg = roll / 16;
		 	pitchDeg = pitch / 16;
		 	yawDeg = heading / 16;
			printf("\nhead: %.2f pitch: %.2f roll: %.2f",yawDeg,pitchDeg,rollDeg);
		 }else{
			 printf("\nFalho");
		 }
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

/*
void app_main() {
	xTaskCreate(task_bno055_normal_mode, "i2c_bno055_task", 1024 * 4,
			(void* ) 0, 10, NULL);
}
*/

