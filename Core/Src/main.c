#include "main.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

#include "stm32l4xx_it.h"
#include <stdbool.h>
#include <stdint.h>

/* Constant parameters */

/* What if we want our display delay to be exact, from the user's perspective? Here we basically
 * Account for the other delay that we do by direct subtraction, but that basically assumes that 
 * button_delay performs a delay exactly once. If we change some logic inside of button_delay,
 * would it be possible to automatically account its total delay consumption (preferrably at compile time), 
 * and then factor that into DISPLAY_DELAY?
 */

#define BUTTON_DELAY 25
#define DISPLAY_DELAY (750 - BUTTON_DELAY)

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

bool PCA_is_high()
{
	return GPIOA->IDR & GPIO_IDR_ID4;
}

void button_delay()
{
	HAL_Delay(BUTTON_DELAY);
}

// check for button state while accounting for button bounce

/* We tried to hide implementation details inside function bodies, in order for the main function
 * to be entirely logic based. At the same time, we wanted our functions to be "pure" or "honest"
 * in the sense that they don't rely on or modify some kind of global state. This also meant that 
 * we tried to isolate and well-define the functions that interact with some kind of external world
 * such as button check and display. 
 * */
bool button_is_pressed()
{
	if (PCA_is_high()) {
		button_delay();

		if (PCA_is_high())
			return true;
	}

	return false;
}

void LED_turn_on()
{
	GPIOC->ODR |= GPIO_ODR_OD0;
	GPIOC->ODR |= GPIO_ODR_OD1;
	GPIOC->ODR |= GPIO_ODR_OD2;
}

void LED_turn_off()
{
	GPIOC->ODR &= ~GPIO_ODR_OD0;
	GPIOC->ODR &= ~GPIO_ODR_OD1;
	GPIOC->ODR &= ~GPIO_ODR_OD2;
}

void count_display(uint8_t count)
{
	LED_turn_off();
	GPIOC->ODR |= ((count) << GPIO_ODR_OD0_Pos);
	HAL_Delay(DISPLAY_DELAY);
}

int main(void)
{
	HAL_Init();
	gpio_init();

	bool LED_is_on = false;
	uint8_t count = 0;

	/* Flags and logic live in main
	 * */
	while (1) {
		if (button_is_pressed()) {
			count_display(count);
			LED_is_on = true;
			count = (count + 1) % 8;
		}
		
		/* Although this is not necessary (we can just repeatedly turn off LED when not needed), we
		 * wanted to see if that is avoidable with boolean flags. This also makes the program more 
		 * closely mimic an actual state machine. 
		 * */
		else if (LED_is_on) {
			LED_turn_off();
		}
	}
}
