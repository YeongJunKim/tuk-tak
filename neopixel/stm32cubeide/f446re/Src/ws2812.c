/*
 * ws2812.c
 *
 *  Created on: Dec 24, 2019
 *      Author: colson
 */




#include "ws2812.h"


#define BIT_PERIOD      (104)
#define BIT_HIGH        (56)
#define BIT_LOW         (28)


#define false 0
#define true 1

BOOL is_init = false;


typedef struct
{
  uint16_t led_cnt;
} ws2812_t;

static uint8_t led_buf[50 + 24*64];


ws2812_t ws2812;
extern TIM_HandleTypeDef htim1;



BOOL ws2812Init(void)
{
  memset(led_buf, 0, sizeof(led_buf));

  is_init = true;


  return true;
}

void ws2812Begin(uint32_t led_cnt)
{
  ws2812.led_cnt = led_cnt;


  HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)led_buf, (50 + 24 * led_cnt) * 1);
}

void ws2812SetColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t r_bit[8];
  uint8_t g_bit[8];
  uint8_t b_bit[8];

  uint32_t offset;


  for (int i=0; i<8; i++)
  {
    if (red & (1<<7))
    {
      r_bit[i] = BIT_HIGH;
    }
    else
    {
      r_bit[i] = BIT_LOW;
    }
    red <<= 1;

    if (green & (1<<7))
    {
      g_bit[i] = BIT_HIGH;
    }
    else
    {
      g_bit[i] = BIT_LOW;
    }
    green <<= 1;

    if (blue & (1<<7))
    {
      b_bit[i] = BIT_HIGH;
    }
    else
    {
      b_bit[i] = BIT_LOW;
    }
    blue <<= 1;
  }

  offset = 50;

  memcpy(&led_buf[offset + index*24 + 8*0], g_bit, 8*1);
  memcpy(&led_buf[offset + index*24 + 8*1], r_bit, 8*1);
  memcpy(&led_buf[offset + index*24 + 8*2], b_bit, 8*1);
}
