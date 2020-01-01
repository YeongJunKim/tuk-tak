/*
 * Neopixel.cpp
 *
 *  Created on: Dec 24, 2019
 *      Author: colson
 */

#include "Neopixel.h"

//Neopixel::Neopixel() {
//	// TODO Auto-generated constructor stub
//
//}
//
//Neopixel::~Neopixel() {
//	// TODO Auto-generated destructor stub
//}

	void Neopixel::begin(void){
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)this->led_buf, size);
	}
	void Neopixel::stop(void){
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	}
	void Neopixel::SetColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
	{
		uint8_t r[8];
		uint8_t g[8];
		uint8_t b[8];
		for (int i = 0; i < 8; i++)
		{
			if(red & (1<<7))
				r[i] = BIT_HIGH;
			else
				r[i] = BIT_LOW;
			red <<= 1;
			if(green & (1<<7))
				g[i] = BIT_HIGH;
			else
				g[i] = BIT_LOW;
			green <<= 1;
			if(blue & (1<<7))
				b[i] = BIT_HIGH;
			else
				b[i] = BIT_LOW;
			blue <<= 1;
		}

		led_buf[(50+/*index * 24 */+ 8 * 0)] = red;
		led_buf[(50+/*index * 24 */+ 8 * 1)] = green;
		led_buf[(50+/*index * 24 */+ 8 * 2)] = blue;
	}
	void Neopixel::SetColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
	{

	}
