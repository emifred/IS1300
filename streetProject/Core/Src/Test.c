/*
 * Test.c
 *
 *  Created on: Dec 5, 2022
 *      Author: emil
 */
#include <stdint.h>
#include "streetFunc.h"
#include "gpio.h"
#include "spi.h"

void Test_Led(void);
void Test_Input(void);


uint8_t stateTest = 1;

void Test_Led(void) {
	for (int i = 0; i <= 4; i++) {
		Flash_Led(i, 'r', 100, 10);
		Flash_Led(i, 'y', 100, 10);
		Flash_Led(i, 'g', 100, 10);
	}
	for (int i = 5; i <= 6; i++) {
		Flash_Led(i, 'r', 100, 10);
		Flash_Led(i, 'g', 100, 10);
		Flash_Led(i, 'b', 100, 10);
	}
	HAL_Delay(1000);
	Led_Clear();
}

void Test_Input(void) {
	while (1) {
		if ((HAL_GPIO_ReadPin(PL1_Switch_GPIO_Port, PL1_Switch_Pin)) == 0) {
			Flash_Led(5, 'r', 100, 3);
			Flash_Led(5, 'g', 100, 3);
			Flash_Led(5, 'b', 100, 3);
			HAL_Delay(300);
		}
		if ((HAL_GPIO_ReadPin(PL2_Switch_GPIO_Port, PL2_Switch_Pin)) == 0) {
			Flash_Led(6, 'r', 100, 3);
			Flash_Led(6, 'g', 100, 3);
			Flash_Led(6, 'b', 100, 3);
			HAL_Delay(300);
		}
		if ((HAL_GPIO_ReadPin(TL1_Car_GPIO_Port, TL1_Car_Pin)) == 0) {
			Flash_Led(1, 'r', 100, 3);
			Flash_Led(1, 'y', 100, 3);
			Flash_Led(1, 'g', 100, 3);
			HAL_Delay(300);
		}
		if ((HAL_GPIO_ReadPin(TL2_Car_GPIO_Port, TL2_Car_Pin)) == 0) {
			Flash_Led(2, 'r', 100, 3);
			Flash_Led(2, 'y', 100, 3);
			Flash_Led(2, 'g', 100, 3);
			HAL_Delay(300);
		}
		if ((HAL_GPIO_ReadPin(TL3_Car_GPIO_Port, TL3_Car_Pin)) == 0) {
			Flash_Led(3, 'r', 100, 3);
			Flash_Led(3, 'y', 100, 3);
			Flash_Led(3, 'g', 100, 3);
			HAL_Delay(300);
		}
		if ((HAL_GPIO_ReadPin(TL4_Car_GPIO_Port, TL4_Car_Pin)) == 0) {
			Flash_Led(4, 'r', 100, 3);
			Flash_Led(4, 'y', 100, 3);
			Flash_Led(4, 'g', 100, 3);
			HAL_Delay(300);
		} else
			Led_Clear();
	}

}

void Test_Switches(void) {
	while (Car_Vertical() == true) {
		Flash_Led(5, 'b', 10, 1);

	}
	Led_Clear();
	while (Car_Horizontal() == true) {
		Flash_Led(6, 'b', 10, 1);

	}
	Led_Clear();
}

void Test_program(void) {
	//Test_Led();

	//Test_Input();
	while(1){
		Test_Switches();
	}
}



