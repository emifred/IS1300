/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : streetFunc.h
  * @brief          : Header for streetFunc.c file.
  *                   This file contains the functions used in the project
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
/* USER CODE END Header */

#ifndef INC_STREETFUNC_C_
#define INC_STREETFUNC_C_
#include <stdbool.h>
#include <stdint.h>



#endif /* INC_STREETFUNC_C_ */



void Led_Clear(void);
/// @brief Function to clear all LEDS
/// Led_Clear();
/// @param void
///
void Blue_Clear1(void);
/// @brief Function to clear the blue light on Crossing 1
/// Blue_Clear();
/// @param void
///
void Blue_Clear2(void);
/// @brief Function to clear the blue light on Crossing 2
/// Blue_Clear();
/// @param void
///

void Led_Toggle(uint8_t n, char state);
/// @brief Function light up a specific colour of a specific traffic/crossing light
/// Led_Toggle(n, 'state');
/// @param1 uint8_t n : Which traffic light is to be toggled
/// @param2 char state : Which colour is to be toggled

void Flash_Led(uint8_t n, char state, uint8_t delay, uint8_t times);
/// @brief Function to toogle on/off LED in a loop
/// Flash_Led(n, 'state', delay, times);
/// @param1 uint8_t delay: Ms delay between blink
/// @param2 uint8_t times: How many times LED should blink

void States(uint8_t state);
/// @brief Function containing the different states of the crossing and the
/// functions to show each of them
/// @details void
/// @details state = 1: Vertical cars have green light
/// @details state = 2: Horizontal cars have green light
/// @details state = 3: Yellow lights are displayed
/// States(state);
/// @tparam uint8_t state: Choosing which state is active
///

bool Crossing_Button1(void);
/// @brief Function for checking if button 1 has been pressed
///
/// @details void
/// Crossing_Button2();
/// @param void
/// @return bool, true if Button 1 has been pressed, otherwise false

bool Crossing_Button2(void);
/// @brief Function for checking if button 2 has been pressed
///
/// @details void
/// Crossing_Button2();
/// @param void
/// @return bool, true if Button 2 has been pressed, otherwise false

bool Car_Horizontal(void);
/// @brief Function for checking if any cars are present on the horizontal road.
///
/// Car_Horizontal(void);
/// @param void
/// @return bool, true if Car is present, otherwise false

bool Car_Vertical(void);
///  @brief Function for checking if any cars are present on the vertical road.
///
/// Car_Vertical();
/// @param void
// /@return bool, true if Car is present, otherwise false

void sendSPIdata(uint8_t data[], uint8_t byteSize);
/// @brief Function for sending data to the shift registers on the shield
///
/// sendSPIdata(data[1], byteSize);
/// @param void
///
