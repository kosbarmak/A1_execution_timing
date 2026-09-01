#include "main.h"
#include "stm32l4xx_hal.h"

int main(void)
{
	HAL_Init();

	// turns on clock to GPIO banks A and C
	// this also powers GPIO so this line is required
	// effectively turn on the heart
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);

	GPIOC->MODER &= ~(0b11 << (0 * 2));
	GPIOC->MODER |= (0b01 << (0 * 2));

	while (1) {
		// set PC0 to high
		// GPIOC->ODR |= GPIO_ODR_OD0;
		GPIOC->ODR |= (1 << 0);

		while(1);

		break; 

		HAL_Delay(1000);
		// GPIOC->ODR &= ~GPIO_ODR_OD0;
		GPIOC->ODR &= ~(1 << 0);
		HAL_Delay(1000);
	} // end while
} // end main
