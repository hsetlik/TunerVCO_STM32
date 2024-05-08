/*
 * neopixel.h
 *
 *  Created on: May 8, 2024
 *      Author: hayden
 */

#ifndef INC_NEOPIXEL_H_
#define INC_NEOPIXEL_H_


#include "stm32g0xx_hal.h"
/* The one-wire interface used by neopixels relies on a PWM signal to encode logical highs and lows
 * At a constant 800kHz frequency, bits are determined to be high or low based on the duty cycle of the
 * high pulse
 * At 800kHz with a timer auto-reload value of 24, this means
 * low = 7
 * high = 15
 *
 *
 * */


// this is how the data we send to DMA should be formatted
typedef struct {
	uint16_t r[8];
	uint16_t g[8];
	uint16_t b[8];
} pixel_data_t;

#define LED_LOW 7
#define LED_HIGH 15

// color packing and blending stuff
uint32_t getRGBColor(uint8_t r, uint8_t g, uint8_t b);
void setColor(pixel_data_t* pixels, uint16_t idx, uint32_t color);
uint32_t lerpColors(uint32_t a, uint32_t b, float t);

// math helper
uint8_t lerp8Bit(uint8_t a, uint8_t b, float t);


#endif /* INC_NEOPIXEL_H_ */
