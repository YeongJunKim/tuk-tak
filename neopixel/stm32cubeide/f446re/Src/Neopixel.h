/*
 * Neopixel.h
 *
 *  Created on: Dec 24, 2019
 *      Author: colson
 *
 *      STM32
 *      80Mhz Timer1
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include "main.h"
#include "stdlib.h"

#define TIMERCLOCK80MHZ 1

#define TIMERCLOCK TIMERCLOCK80MHZ
#if(TIMERCLOCK == TIMERCLOCK80MHZ)
#define offset 50
#endif

#define BIT_HIGH 1
#define BIT_LOW 0

extern TIM_HandleTypeDef htim1;

class Neopixel
{
public:
	uint8_t *led_buf;
	uint32_t size;

	Neopixel(){

	};
	Neopixel(uint32_t led_cnt_, uint8_t colors_)
	{
		size = (sizeof(uint8_t) * led_cnt_ * colors_) + 50;
		led_buf = (uint8_t *)malloc(size);
		for(uint32_t i = 0; i < size; i++)
		{
			led_buf[i] = 0x00;
		}
	};
	~Neopixel();

	void begin(void);
	void stop(void);
	void SetColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue);
	void SetColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
};

#endif /* NEOPIXEL_H_ */
