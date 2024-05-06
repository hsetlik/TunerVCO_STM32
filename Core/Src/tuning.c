/*
 * tuning.c
 *
 *  Created on: May 3, 2024
 *      Author: hayden
 */

#include "tuning.h"
#include <math.h>
#include <ssd1306.h>

void triggerTuningUpdate(uint32_t* buffer, uint16_t* head) {
	uint32_t val = HAL_GetTick();
	buffer[*head] = val;
	*head = (*head + 1) % TUNING_BUF_SIZE;
}


uint32_t getTuningBufferValue(uint32_t* buffer, uint16_t* head, uint16_t idx) {

	idx = (*head + idx) % TUNING_BUF_SIZE;
	return buffer[idx];
}


float getCurrentHz(uint32_t* buffer) {
	float sum = 0.0f;
	for(uint8_t idx = 1; idx < TUNING_BUF_SIZE; idx++){
		uint32_t diff = buffer[idx] - buffer[idx - 1];
		sum += (float)diff;
	}
	float avgIntervalMs = sum / (float)TUNING_BUF_SIZE;
	return 1000.0f / avgIntervalMs;
}

// initialize an array of pitches for each MIDI note
void initMidiNotePitches() {
	for(uint8_t note = 0; note < NUM_MIDI_NOTES; note++)
	{
		float fNum = (float)note - 69.0f;
		midiNotePitches[note] = 440.0f * powf(SEMITONE_RATIO, fNum);
	}
}

uint8_t nearestMidiNote(float hz){
	if(!midiNotesInitialized){
		initMidiNotePitches();
		midiNotesInitialized = true;
	}

	if(hz <= midiNotePitches[0])
		return 0;
	if(hz >= midiNotePitches[NUM_MIDI_NOTES - 1])
		return midiNotePitches[NUM_MIDI_NOTES - 1];
	// binary search
	uint8_t i = 0;
	uint8_t j = NUM_MIDI_NOTES;
	uint8_t mid = 0;
	while (i < j) {
		mid = (i + j) / 2;
		// check left half
		if(hz < midiNotePitches[mid]) {
			if(mid > 0 && hz >= midiNotePitches[mid - 1]) {
				if(lowIsCloser(midiNotePitches[mid - 1], midiNotePitches[mid], hz))
					return mid - 1;
				return mid;
			}
			j = mid;
		} else { // check right half
			if(mid < (NUM_MIDI_NOTES - 1) && hz < midiNotePitches[mid + 1]) {
				if(lowIsCloser(midiNotePitches[mid], midiNotePitches[mid + 1], hz))
					return mid;
				return mid + 1;
			}
			i = mid + 1;
		}
	}
	return mid;
}


int16_t getTuningErrorCents(float hz, uint8_t midiTarget){
	if(!midiNotesInitialized){
		initMidiNotePitches();
		midiNotesInitialized = true;
	}
	if(hz <= midiNotePitches[midiTarget]){ // we're flat
		float diff = midiNotePitches[midiTarget] - hz;
		float halfstep = midiNotePitches[midiTarget] - midiNotePitches[midiTarget - 1];
		return (int16_t)((halfstep / diff) * -100.0f);
	} else {
		float diff = hz - midiNotePitches[midiTarget];
		float halfstep = midiNotePitches[midiTarget + 1] - midiNotePitches[midiTarget];
		return (int16_t)((halfstep / diff) * 100.0f);
	}
}

bool lowIsCloser(float low, float hi, float target) {
	return (target - low) >= (hi - target);
}


void displayTuning(float hz){
	static char* noteNames[] = {
			"A",
			"Bb",
			"B",
			"C",
			"Db",
			"D",
			"Eb",
			"E",
			"F",
			"Gb",
			"G"
	};

	uint8_t x = (128 - 16) / 2;
	uint8_t y = (64 - 26) / 2;

	uint8_t target = nearestMidiNote(hz);

	// draw the note name
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(noteNames[target % 12], Font_16x24, White);

	//display the error
	int tuningError = getTuningErrorCents(hz, target);
	uint8_t x1, y1, x2, y2;
	if(tuningError < (0 - IN_TUNE_THRESHOLD)) { // flat
		x2 = SSD1306_WIDTH / 2;
		float fDist = (float)(tuningError * -1) / 100.0f;
		x1 = x2 - (uint8_t)(fDist * (float)x2);
		y1 = 0;
		y2 = SSD1306_HEIGHT;

		ssd1306_InvertRectangle(x1, y1, x2, y2);
	}
	else if (tuningError > IN_TUNE_THRESHOLD){ // sharp
		x1 = SSD1306_WIDTH / 2;
		float fDist = (float)(tuningError) / 100.0f;
		x2 = x1 + (uint8_t)(fDist * (float)x1);
		y1 = 0;
		y2 = SSD1306_HEIGHT;

		ssd1306_InvertRectangle(x1, y1, x2, y2);

	}

}

