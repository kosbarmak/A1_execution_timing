#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "main.h"
#include <math.h>

typedef int var_type;

void SystemClock_Config(void);
var_type TestFunction(var_type num);

int main(void)
{
	var_type main_var;
	;

	HAL_Init();
	SystemClock_Config();

	// Configure PC0 and PC1 as GPIO outputs, push-pull, with no pull-up/down
	// and high speed.
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1);
	GPIOC->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0;
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);
	GPIOC->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED0_Pos) | (3 << GPIO_OSPEEDR_OSPEED1_Pos);

	// Preset PC0 and PC1 to 0.
	GPIOC->BRR = GPIO_PIN_0;

	while (1) { // Infinite loop to avoid program exit.
		main_var++; // Added to eliminate the unused-variable warning.
		GPIOC->BSRR = GPIO_PIN_0; // Turn on PC0.
		main_var = TestFunction(15); // Test function being timed.
		GPIOC->BRR = GPIO_PIN_0; // Turn off PC0.
	}
}

var_type TestFunction(var_type num)
{
	var_type test_var;

	// Insert the function being measured here (for example: test_var = num;).
	return test_var;
}
