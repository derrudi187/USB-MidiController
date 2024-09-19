/*
 * Name: config.h
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 15.09.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
#ifndef __config_new_h_included__
#define __config_new_h_included__
/* Incremental encoder config
 *
 * Number of connected incremental encoders, range of 1-16
 * If NUMBER_INC_EC not defined or set to zero, incremental encoders are disable */
#define INC_ENC_NUMBER 16
/* Channel number for send message, range 0-15(channel 1-16)
 * If not defined, default is midi-channel 1
 * WARNING: if this channel same with ADC_MIDI_CHANNEL, the control change number(cc) can do overlap */
//#define INC_ENC_MIDI_CHANNEL 0
/* Incremental encoder mode
 * If INC_ENC_MODE set to 0: sends 1 while rotate clockwise otherwise 127 counterclockwise
 * If INC_ENC_MODE set to 1: sends value + INC_INC_STEP while rotate clockwise otherwise value - INC_ENC_STEP counterclockwise(value starts with zero)
 * If not defined, default is mode 0 */
//#define INC_ENC_MODE 0
/* Incremental encoder maximum value, range 1-127
 * If not defined, default is 127 */
//#define INC_ENC_MAX 127
/* Incremental encoder step per pulse, range 1-10
 * If not defined, default is 1 */
//#define INC_ENC_STEP 1
/* Control change number(cc) for each encoder, range 0-119
 * 120-127 are reserved as channel mode message */
#define INC_ENC1	0
#define INC_ENC2	1
#define INC_ENC3	2
#define INC_ENC4	3
#define INC_ENC5	4
#define INC_ENC6	5
#define INC_ENC7	6
#define INC_ENC8	7
#define INC_ENC9	8
#define INC_ENC10	9
#define INC_ENC11	10
#define INC_ENC12	11
#define INC_ENC13	12
#define INC_ENC14	13
#define INC_ENC15	14
#define INC_ENC16	15
/* Misc for incremental encoder */
#ifdef INC_ENC_NUMBER
	#if INC_ENC_NUMBER > 16
		#error "INC_ENC_NUMBER out of range, config.h"
	#else
		#if INC_ENC_NUMBER < 1
			#undef INC_ENC_NUMBER
		#else
			#if INC_ENC_NUMBER < 5
				#define INC_ENC_STORAGE 1
			#endif
			#if INC_ENC_NUMBER < 9
				#define INC_ENC_STORAGE 2
			#endif
			#if INC_ENC_NUMBER < 13
				#define INC_ENC_STORAGE 3
			#endif
			#ifndef INC_ENC_STORAGE
				#define INC_ENC_STORAGE 4
			#endif
			#ifndef INC_ENC_MIDI_CHANNEL
				#define INC_ENC_MIDI_CHANNEL 0
			#else
				#if INC_ENC_MIDI_CHANNEL < 0
				#if INC_ENC_MIDI_CHANNEL > 15
					#error "INC_ENC_MIDI_CHANNEL out of range, config.h"
				#endif
				#endif
			#endif
			#ifndef INC_ENC_MODE
				#define INC_ENC_MODE 0
			#else
				#if INC_ENC_MODE < 0
				#if INC_ENC_MODE > 1
					#error "INC_ENC_MODE out of range, config.h"
				#endif
				#endif
			#endif
			#ifndef INC_ENC_MAX
				#define INC_ENC_MAX 127
			#else
				#if INC_ENC_MAX < 1
				#if INC_ENC_MAX > 127
					#error "INC_ENC_MAX out of range, config.h"
				#endif
				#endif
			#endif
			#ifndef INC_ENC_STEP
				#define INC_ENC_STEP 1
			#else
				#if INC_ENC_STEP < 1
				#if INC_ENC_STEP > 10
					#error "INC_ENC_STEP out of range, config.h"
				#endif
				#endif
			#endif
		#endif
	#endif
#endif
#endif /* __config_new_h_included__*/