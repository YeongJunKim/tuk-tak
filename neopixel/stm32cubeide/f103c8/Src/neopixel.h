/*
 * neopixel.h
 *
 *  Created on: Oct 8, 2019
 *      Author: colson
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include "main.h"
#include "stdlib.h"

#define NEOPIXEL_6061_
#ifdef NEOPIXEL_MINI

#else
#define NEOPIXEL_6061_
#endif

#define BIT_PERIOD 	103
#define BIT_HIGH	49
#define BIT_LOW		25

uint8_t is_init = 0;
uint16_t led_cnt = 0;

#ifdef NEOPIXEL_6061
//uint8_t *led_buf[70+32];
uint8_t *led_buf;
#else
uint8_t *led_buf;
#endif
extern TIM_HandleTypeDef htim1;

void neopixel_init(int cnt)
{
	led_buf = (uint8_t*)malloc(sizeof(uint8_t)*24*cnt+70);
//	for(int i = 0; i < 70; i++)
//		led_buf[i] = 40;

}
void neopixel_pause(void)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
}
void neopixel_begin(void)
{
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)led_buf, sizeof(led_buf));
}
void neopixel_SetColor(uint32_t cnt, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
	uint32_t cnt_ = cnt - 1;
	for(int i = 0; i < 8; i ++)
	{

#ifdef NEOPIXEL_6061
		uint16_t index_red 		= 69+32*cnt_+8-i;
		uint16_t index_green 	= 69+32*cnt_+16-i;
		uint16_t index_blue 	= 69+32*cnt_+24-i;
		uint16_t index_white 	= 69+32*cnt_+32-i;
		if(green >> i & 0x01)
			led_buf[index_red] = BIT_HIGH;
		else
			led_buf[index_red] = BIT_LOW;
		if(red >> i & 0x01)
			led_buf[index_green] = BIT_HIGH;
		else
			led_buf[index_green] = BIT_LOW;
		if(blue >> i & 0x01)
			led_buf[index_blue] = BIT_HIGH;
		else
			led_buf[index_blue] = BIT_LOW;
		if(white >> i & 0x01)
			led_buf[index_white] = BIT_HIGH;
		else
			led_buf[index_white] = BIT_LOW;
#else
		uint16_t index_green 		= 69+(24*cnt_)+8-i;
		uint16_t index_red 			= 69+(24*cnt_)+16-i;
		uint16_t index_blue 		= 69+(24*cnt_)+24-i;
		if(green >> i & 0x01)
			led_buf[index_red] = BIT_HIGH;
		else
			led_buf[index_red] = BIT_LOW;
		if(red >> i & 0x01)
			led_buf[index_green] = BIT_HIGH;
		else
			led_buf[index_green] = BIT_LOW;
		if(blue >> i & 0x01)
			led_buf[index_blue] = BIT_HIGH;
		else
			led_buf[index_blue] = BIT_LOW;
#endif

	}
}


#endif /* NEOPIXEL_H_ */
