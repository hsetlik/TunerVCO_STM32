/*
 * neopixel.c
 *
 *  Created on: May 8, 2024
 *      Author: hayden
 */

#include "neopixel.h"


uint32_t getRGBColor(uint8_t r, uint8_t g, uint8_t b){
	uint32_t color = 0;
	color = color | ((uint32_t)r);
	color = color | ((uint32_t)g << 8);
	color = color | ((uint32_t)b << 16);
	return color;
}

void setColor(pixel_data_t* pixels, uint16_t idx, uint32_t color){
	uint8_t r = (uint8_t)color;
	uint8_t g = (uint8_t)(color >> 8);
	uint8_t b = (uint8_t)(color >> 16);

	for(uint8_t i = 0; i < 8; i++){
		// check the bits on each channel

		// red
		if(r & (1 << i)){
			(pixels + idx)->r[i] = LED_HIGH;
		} else {
			(pixels + idx)->r[i] = LED_LOW;
		}

		//green
		if(g & (1 << i)){
			(pixels + idx)->g[i] = LED_HIGH;
		} else {
			(pixels + idx)->g[i] = LED_LOW;
		}

		//blue
		if(b & (1 << i)){
			(pixels + idx)->b[i] = LED_HIGH;
		} else {
			(pixels + idx)->b[i] = LED_LOW;
		}

	}

}

uint32_t lerpColors(uint32_t a, uint32t b, float t){

	uint8_t rA = (uint8_t)a;
	uint8_t gA = (uint8_t)(a >> 8);
	uint8_t bA = (uint8_t)(a >> 16);

	uint8_t rB = (uint8_t)b;
	uint8_t gB = (uint8_t)(b >> 8);
	uint8_t bB = (uint8_t)(b >> 16);

	return getRGBColor(lerp8Bit(rA, rB, t), lerp8Bit(gA, gB, t), lerp8Bit(bA, bB, t));
}

uint8_t lerp8Bit(uint8_t a, uint8_t b, float t){
	float fVal = (float)(b - a) * t;
	fVal += (float)a;
	return (uint8_t)fVal;
}
