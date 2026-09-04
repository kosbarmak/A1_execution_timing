#include "main.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

#include "stm32l4xx_it.h"
#include <stdbool.h>

#define GPIO_MODER_MODE_OUTPUT 0b01

void gpio_init()
{
	// turns on clock to GPIO banks A and C
	// this also powers GPIO so this line is required
	// effectively turn on the heart
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);

	// Set PC0-2 as LED power oudput
	GPIOC->MODER &= ~(GPIO_MODER_MODE0_Msk);
	GPIOC->MODER |= (0b0000000000000000000000000000000000000000000000000000000000000001 << GPIO_MODER_MODE0_Pos);

	GPIOC->MODER &= ~(GPIO_MODER_MODE1_Msk);
	GPIOC->MODER |= (0b01 << GPIO_MODER_MODE1_Pos);

	GPIOC->MODER &= ~(GPIO_MODER_MODE2_Msk);
	GPIOC->MODER |= (0b01 << GPIO_MODER_MODE2_Pos);

	// set PA4 as button as input
	GPIOA->MODER &= ~(GPIO_MODER_MODE4_Msk);
	GPIOA->MODER |= (GPIO_MODE_INPUT << GPIO_MODER_MODE4_Pos);

	// set PA4 as pulldown
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4_Msk);
	GPIOA->PUPDR |= (GPIO_PULLDOWN << GPIO_PUPDR_PUPD4_Pos);
}

bool button_pressed()
{
	return GPIOA->IDR & GPIO_IDR_ID4;
}

void LED_on()
{
	GPIOC->ODR |= GPIO_ODR_OD0;
	GPIOC->ODR |= GPIO_ODR_OD1;
	GPIOC->ODR |= GPIO_ODR_OD2;
}

void LED_off()
{
	GPIOC->ODR &= ~GPIO_ODR_OD0;
	GPIOC->ODR &= ~GPIO_ODR_OD1;
	GPIOC->ODR &= ~GPIO_ODR_OD2;
}

int main(void)
{
	HAL_Init();
	gpio_init();

	const int button_delay = 25;

	while (1) {
		// set PC0 to high
		// GPIOC->ODR |= GPIO_ODR_OD0;
		// GPIOC->ODR |= (1 << 0);

		if (button_pressed()) {
			HAL_Delay(button_delay);

			if (button_pressed()) {
				LED_on();
			}

			while(button_pressed()) {
				HAL_Delay(button_delay);
			}
		}

		LED_off();

		// // button is still not pressed
		// while (!(GPIOA->IDR & GPIO_IDR_ID4))
		// 	HAL_Delay(button_delay);
		//
		//
		// HAL_Delay(button_delay);
		//
		// while (GPIOA->IDR & GPIO_IDR_ID4) {
		// 	HAL_Delay(button_delay);
		// }
		//
		//
	} // end while
} // end main
