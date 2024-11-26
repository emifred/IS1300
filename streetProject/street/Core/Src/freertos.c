/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file          : freertos.c
  * @brief        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "streetFunc.h"
#include "Test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	Vertical, Horizontal, Yellow
} states1;

typedef enum {
	Default, Button1, Button2, Horizontal_Car, Vertical_Car
} states2;

static states1 state;
static states2 GPIO_State;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
bool Waiting_Cars = false; /*! Control variable to see if a car is waiting at a red light*/

uint32_t toggleFreq = 100; /*!Toogle frequency for pedestrian light*/
uint32_t pedestrianDelay = 6000; /*!Time from pedestrian button pressed to green light*/
uint32_t walkingDelay = 15000; /*!Delay for pedestrian crossing*/
uint32_t orangeDelay = 3000; /*!Delay for orange light time*/
uint32_t greenDelay = 6000; /*!The default delay between green-yellow red-yellow when no cars are present*/
uint32_t redDelayMax = 5000; /*!The maximum delay a car has a red light when arriving at the crossing when cars going vertically are present.*/
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime2,
};
/* Definitions for GPIO_Button1 */
osThreadId_t GPIO_Button1Handle;
const osThreadAttr_t GPIO_Button1_attributes = {
  .name = "GPIO_Button1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for GPIO_Switches */
osThreadId_t GPIO_SwitchesHandle;
const osThreadAttr_t GPIO_Switches_attributes = {
  .name = "GPIO_Switches",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Timer_Func */
osThreadId_t Timer_FuncHandle;
const osThreadAttr_t Timer_Func_attributes = {
  .name = "Timer_Func",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for GPIO_Button2 */
osThreadId_t GPIO_Button2Handle;
const osThreadAttr_t GPIO_Button2_attributes = {
  .name = "GPIO_Button2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for event */
osMutexId_t eventHandle;
const osMutexAttr_t event_attributes = {
  .name = "event"
};
/* Definitions for buttonsSem */
osSemaphoreId_t buttonsSemHandle;
const osSemaphoreAttr_t buttonsSem_attributes = {
  .name = "buttonsSem"
};
/* Definitions for switchesSem */
osSemaphoreId_t switchesSemHandle;
const osSemaphoreAttr_t switchesSem_attributes = {
  .name = "switchesSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Button_Task1(void *argument);
void Switches(void *argument);
void timer(void *argument);
void Button_Task2(void *argument);
void Stall_Function(uint32_t delay);
void Button1_TimerHandler(void);
void Button2_TimerHandler(void);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of event */
  eventHandle = osMutexNew(&event_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of buttonsSem */
  buttonsSemHandle = osSemaphoreNew(1, 1, &buttonsSem_attributes);

  /* creation of switchesSem */
  switchesSemHandle = osSemaphoreNew(1, 1, &switchesSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of GPIO_Button1 */
  GPIO_Button1Handle = osThreadNew(Button_Task1, NULL, &GPIO_Button1_attributes);

  /* creation of GPIO_Switches */
  GPIO_SwitchesHandle = osThreadNew(Switches, NULL, &GPIO_Switches_attributes);

  /* creation of Timer_Func */
  Timer_FuncHandle = osThreadNew(timer, NULL, &Timer_Func_attributes);

  /* creation of GPIO_Button2 */
  GPIO_Button2Handle = osThreadNew(Button_Task2, NULL, &GPIO_Button2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	/*!Highest priority task which cycles through the different states
	Depending on the delay in Timer-task*/
	GPIO_State = Default; /*!Set the GPIO to default, meaning no user input at initialisation*/
	/* Infinite loop */
	for (;;) {
		state = Vertical;
		States(1); /*!Display the LEDS for when cars going vertically has the green light*/
		vTaskDelay(1); /*! Small delay to prevent loss of data to the LEDS*/
		vTaskSuspend(defaultTaskHandle); /*! Suspend the default program to continue on to the Timer task*/

		state = Yellow; /*! Switch the state to show the yellow lights*/
		States(3);
		vTaskSuspend(defaultTaskHandle);

		state = Horizontal;
		States(2);
		vTaskDelay(1);
		vTaskSuspend(defaultTaskHandle);

		state = Yellow;
		States(3);
		vTaskDelay(1);
		vTaskSuspend(defaultTaskHandle);

	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Button_Task1 */
/**
* @brief Function implementing the GPIO_Button1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Button_Task1 */
void Button_Task1(void *argument)
{
	/* USER CODE BEGIN Button_Task1 */
	/* Infinite loop */
	for (;;) {
		if (Crossing_Button1() == true && state != Horizontal) /*! If Button 1 is pressed and the pedestrian light isn't already green*/
			while (state != Horizontal) { /*! While the pedestrian crossing isn't green*/
				Flash_Led(6, 'b', 1, 1); /*! Flash the corresponding blue light*/
				vTaskDelay(pdMS_TO_TICKS(toggleFreq)); /*! Delay the task to receive a delay between blinks while letting the other tasks continue to work*/
				if ((osSemaphoreAcquire(buttonsSemHandle, 1) == osOK)) /*! If the other crossing isn't in operation change the state to effect the delay*/
					GPIO_State = Button1;
			}
		Blue_Clear1(); /*! Function to clear the Blue LED to avoid any lingering light*/

	}
	/* USER CODE END Button_Task1 */
}

/* USER CODE BEGIN Header_Switches */
/**
* @brief Function implementing the GPIO_Switches thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Switches */
void Switches(void *argument)
{
	/* USER CODE BEGIN Switches */
	/* Infinite loop */
	for (;;) {
		if ((osSemaphoreAcquire(buttonsSemHandle, 1) == osOK)) { /*! If the buttons are not in use*/
			osSemaphoreRelease(buttonsSemHandle);
			if (Car_Vertical() == true && Car_Horizontal() == true
					&& GPIO_State != (Button1 || Button2)) { /*! If there is a car wating in both ways, and there are no pedestrians crossing*/
				GPIO_State = Default; /*! Change the state to default altering lights but with a new delay*/
				Waiting_Cars = true; /*! Telling the timer task that there are cars waiting in both directions.*/
			}

			else if (Car_Horizontal() == true && Car_Vertical() == false
					&& GPIO_State != (Button1 || Button2)) { /*! If a car is waiting in the horizontal direction and there are no pedestrians*/
				GPIO_State = Horizontal_Car;
			}

			else if (Car_Vertical() == true && Car_Horizontal() == false
					&& GPIO_State != (Button1 || Button2)) { /*! If a car is waiting in the vertical direction and there are no pedestrians*/
				GPIO_State = Vertical_Car;
			} else if (GPIO_State != (Button1 || Button2)) { /*! If there are no cars, and no pedestrians*/
				GPIO_State = Default;
				Waiting_Cars = false; /*! Clear waiting cars because there are none*/
			}
		}
	}
	/* USER CODE END Switches */
}

/* USER CODE BEGIN Header_timer */
/**
* @brief Function implementing the Timer_Func thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_timer */
void timer(void *argument)
{
	/* USER CODE BEGIN timer */
	/* Infinite loop */
	for (;;) {
		switch (GPIO_State) {
		case Default: {
			if (state == Yellow) {
				Stall_Function(orangeDelay);
			} else if (Waiting_Cars == true) {
				Stall_Function(redDelayMax);
			} else {
				Stall_Function(greenDelay);
			}
			break;
		}
		case Button1: { //if any of buttons been pressed
			Button1_TimerHandler();

			break;
		}
		case Button2: { //if any of buttons been pressed
			Button2_TimerHandler();
			break;
		}
		case Horizontal_Car: {
			if (state == Horizontal) {
				vTaskDelay(10);
			} else {
				vTaskResume(defaultTaskHandle);

			}
			break;
		}
		case Vertical_Car: {
			if (state == Vertical) {
				vTaskDelay(10);
			} else {
				vTaskResume(defaultTaskHandle);

			}

			break;
		}

		}
	}

	/* USER CODE END timer */
}

/* USER CODE BEGIN Header_Button_Task2 */
/**
* @brief Function implementing the GPIO_Button2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Button_Task2 */
void Button_Task2(void *argument)
{
	/* USER CODE BEGIN Button_Task2 */
	/* Infinite loop */
	for (;;) {
		if (Crossing_Button2() == true && state != Vertical) {
			while (state != Vertical) {
				Flash_Led(5, 'b', 1, 1);
				vTaskDelay(pdMS_TO_TICKS(toggleFreq));
				if ((osSemaphoreAcquire(buttonsSemHandle, 1) == osOK))
					GPIO_State = Button2;
			}
			Blue_Clear2();
		}
	}
	/* USER CODE END Button_Task2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
* @brief Function for delaying Timer task and starting the main task
* @param Delay time stated in miliseconds
* @retval None
*/
void Stall_Function(uint32_t delay) {
	vTaskDelay(pdMS_TO_TICKS(delay));
	vTaskResume(defaultTaskHandle);
}
/**
* @brief Function handeling delays when button 1 has been pressed
* @param argument: Not used
* @retval None
*/
void Button1_TimerHandler(void) {
	if (state == Yellow) { //if the button was pressed when there was a orange light
		Stall_Function(orangeDelay);
	} else if (state == Horizontal) {
		vTaskDelay(pdMS_TO_TICKS(walkingDelay));
		osSemaphoreRelease(buttonsSemHandle);
		GPIO_State = Default;
	} else {
		Stall_Function(pedestrianDelay - orangeDelay);
		Stall_Function(orangeDelay);
		vTaskDelay(pdMS_TO_TICKS(walkingDelay));
		osSemaphoreRelease(buttonsSemHandle);
		GPIO_State = Default;
	}
}
/**
* @brief Function handling delays when button 2 has been pressed
* @param argument: Not used
* @retval None
*/
void Button2_TimerHandler(void) {
	if (state == Yellow) { //if the button was pressed when there was a orange light
		Stall_Function(orangeDelay);
	} else if (state == Vertical) {
		vTaskDelay(pdMS_TO_TICKS(walkingDelay));
		osSemaphoreRelease(buttonsSemHandle);
		GPIO_State = Default;
	} else {
		Stall_Function(pedestrianDelay - orangeDelay);
		Stall_Function(orangeDelay);
		vTaskDelay(pdMS_TO_TICKS(walkingDelay));
		osSemaphoreRelease(buttonsSemHandle);
		GPIO_State = Default;
	}
}
/* USER CODE END Application */

