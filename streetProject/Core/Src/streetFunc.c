
 /******************************************************************************
 * File Name          : streetFunc.c
 * Description        : File for functions used in the project
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


#include <stdint.h>
#include "streetFunc.h"
#include "gpio.h"
#include "spi.h"


uint32_t led[] = {0b000000000000000000000000000000000};

void Led_Clear(void){
	led[0] = 0x00000000;
	sendSPIdata(&led[0], 3);
}

void Blue_Clear1(void){
	led[0]&= ~(1 << 13);
	sendSPIdata(&led[0], 3);
	return;
}
void Blue_Clear2(void){
	led[0]&= ~(1 << 21);
	sendSPIdata(&led[0], 3);
	return;
}

void Led_Toggle(uint8_t n, char state){

	switch(n){
		case 3:
			switch(state){
			case 'g':
				led[0]^= 1 << 2;
				led[0]&= ~(1 << 1);
				led[0]&= ~(1 << 0);
				break;
			case 'y':
				led[0]^= 1 << 1;
				led[0]&= ~(1 << 2);
				led[0]&= ~(1 << 0);
				break;
			case 'r':
				led[0]^= 1 << 0;
				led[0]&= ~(1 << 2);
				led[0]&= ~(1 << 1);
				break;
			}
			break;
		case 4:
				switch(state){
				case 'g':
					led[0]^= 1 << 5;
					led[0]&= ~(1 << 4);
					led[0]&= ~(1 << 3);
					break;
				case 'y':
					led[0]^= 1 << 4;
					led[0]&= ~(1 << 5);
					led[0]&= ~(1 << 3);
					break;
				case 'r':
					led[0]^= 1 << 3;
					led[0]&= ~(1 << 5);
					led[0]&= ~(1 << 4);
					break;
				}
		break;
		case 2:
			switch(state){
			case 'g':
				led[0]^= 1 << 10;
				led[0]&= ~(1 << 9);
				led[0]&= ~(1 << 8);
				break;
			case 'y':
				led[0]^= 1 << 9;
				led[0]&= ~(1 << 8);
				led[0]&= ~(1 << 10);
				break;
			case 'r':
				led[0]^= 1 << 8;
				led[0]&= ~(1 << 10);
				led[0]&= ~(1 << 9);
				break;
			}
		break;
		case 1:
			switch(state){
			case 'g':
				led[0]^= 1 << 18;
				led[0]&= ~(1 << 17);
				led[0]&= ~(1 << 16);
				break;
			case 'y':
				led[0]^= 1 << 17;
				led[0]&= ~(1 << 16);
				led[0]&= ~(1 << 18);
				break;
			case 'r':
				led[0]^= 1 << 16;
				led[0]&= ~(1 << 17);
				led[0]&= ~(1 << 18);
				break;
			}
		break;
		case 6:
			switch(state){
			case 'r':
				led[0]^= 1 << 11;
				led[0]&= ~(1 << 12);
				break;
			case 'g':
				led[0]^= 1 << 12;
				led[0]&= ~(1 << 11);
				break;
			case 'b':
				led[0]^= 1 << 13;
			}
			break;
		case 5:
			switch(state){
			case 'r':
				led[0]^= 1 << 19;
				led[0]&= ~(1 << 20);
				break;
			case 'g':
				led[0]^= 1 << 20;
				led[0]&= ~(1 << 19);
				break;
			case 'b':
				led[0]^= 1 << 21;
			}
		/*case 7:
			switch(state){
			case 'b':
				led[0]^= 1 << 13;
				led[0]^= 1 << 22;
				break;
			}*/
			break;
	}



}

void Flash_Led(uint8_t n, char state, uint8_t delay, uint8_t times){
	for(int i = 0; i <= times; i++){
		Led_Toggle(n, state);
		HAL_Delay(delay);
	}
	Led_Toggle(n, state);
	sendSPIdata(&led[0], 3);

}

void States(uint8_t state){
	switch(state){
	case 1:{
		//Bitshift the crossings first because the only have two states,
		//which makes them a bit hard to do with Led_Toggle in 3 states. Led_Toggle will send their values.
		led[0] |= 1 << 11;
		led[0]&= ~(1 << 12);
		led[0] |= 1 << 20;
		led[0]&= ~(1 << 19);

		Led_Toggle(1, 'r');
		Led_Toggle(2, 'g');
		Led_Toggle(3, 'r');
		Led_Toggle(4, 'g');


		sendSPIdata(&led[0], 3);
	break;
	}
	case 2:{
		led[0] |= 1 << 19;
		led[0]&= ~(1 << 20);
		led[0] |= 1 << 12;
		led[0]&= ~(1 << 11);

		Led_Toggle(1, 'g');
		Led_Toggle(2, 'r');
		Led_Toggle(3, 'g');
		Led_Toggle(4, 'r');
		sendSPIdata(&led[0], 3);


		break;
	}
	case 3:{
		led[0] |= 1 << 19;
		led[0]&= ~(1 << 20);
		led[0] |= 1 << 11;
		led[0]&= ~(1 << 12);

		Led_Toggle(1, 'y');
		Led_Toggle(2, 'y');
		Led_Toggle(3, 'y');
		Led_Toggle(4, 'y');
		sendSPIdata(&led[0], 3);

		break;
	}
	}
}


bool Crossing_Button2(void){
	if ((HAL_GPIO_ReadPin(PL1_Switch_GPIO_Port, PL1_Switch_Pin)) == 0){
		return true;
	}
	else
		return false;
}

bool Crossing_Button1(void){
	if ((HAL_GPIO_ReadPin(PL2_Switch_GPIO_Port, PL2_Switch_Pin)) == 0){
		return true;
	}
	else
		return false;
}

bool Car_Vertical(void){
	if ((HAL_GPIO_ReadPin(TL2_Car_GPIO_Port, TL2_Car_Pin)) == 0 || (HAL_GPIO_ReadPin(TL4_Car_GPIO_Port, TL4_Car_Pin)) == 0){
		return true;
	}
	else
		return false;
}
bool Car_Horizontal(void){
	if ((HAL_GPIO_ReadPin(TL1_Car_GPIO_Port, TL1_Car_Pin)) == 0 || (HAL_GPIO_ReadPin(TL3_Car_GPIO_Port, TL3_Car_Pin)) == 0){
		return true;
	}
	else
		return false;
}

void sendSPIdata(uint8_t data[], uint8_t byteSize){

	HAL_SPI_Transmit(&hspi3, &data[0], byteSize, 100);



	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);

}
