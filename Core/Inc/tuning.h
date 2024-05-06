/*
 * tuning.h
 *
 *  Created on: May 3, 2024
 *      Author: hayden
 */

#ifndef TUNING_H_
#define TUNING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g0xx_hal.h"
#include <stdbool.h>

// some constants
#define TUNING_BUF_SIZE 200
#define SEMITONE_RATIO 1.05946309436f


// tuning buffer stuff
void triggerTuningUpdate(uint32_t* buffer, uint16_t* head);
uint32_t getTuningBufferValue(uint32_t* buffer, uint16_t* head, uint16_t idx);
float getCurrentHz(uint32_t* buffer);

// tuning description/display stuff
#define NUM_MIDI_NOTES 128
static float midiNotePitches[NUM_MIDI_NOTES];
static bool midiNotesInitialized;
void initMidiNotePitches();
uint8_t nearestMidiNote(float hz);
int16_t getTuningErrorCents(float hz, uint8_t midiTarget);

// little math helper
bool lowIsCloser(float low, float hi, float target);






#ifdef __cplusplus
}
#endif

#endif /* INC_TUNING_H_ */
