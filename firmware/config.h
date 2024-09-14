/*
 * Name: config.h
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 03.09.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
#ifndef __config_h_included__
#define __config_h_included__
/* Incemental encoder config
 * Number of incremental encoders in range of 1-16
 * If INC_ENC not defined or set to zero, incremental encoders are disabled */
#define INC_ENC 16
/* #define INC_MAX 127	        // Maximum value, range 1-127, if not defined, default: 127 */
/* #define INC_STEP 1	        // Step per pulse, range 1-10, if not defined, default: 1 */
#define INC_MIDI_CHANNEL 1   // Channel number , range 0-15 (channel 1-16), if not defined, default: 0
/* control change number for each encoder, range 0-119, 120-127 are reserved as channel mode message */
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
/* Key config
 * Number of Keys in range of 1-32
 * If not defined or set to zero, key are disable */
#define KEY 32
#define KEY_MIDI_CHANNEL 1   // Channel number, range 0-15 (channel 1-16), if not define, defualt: 0
/* Note number for each key, range 0-127 */
#define KEY1	0
#define KEY2	1
#define KEY3	2
#define KEY4	3
#define KEY5	4
#define KEY6	5
#define KEY7	6
#define KEY8	7
#define KEY9	8
#define KEY10	9
#define KEY11	10
#define KEY12	11
#define KEY13	12
#define KEY14	13
#define KEY15	14
#define KEY16	15
#define KEY17	16
#define KEY18	17
#define KEY19	18
#define KEY20	19
#define KEY21	20
#define KEY22	21
#define KEY23	22
#define KEY24	23
#define KEY25	24
#define KEY26	25
#define KEY27	26
#define KEY28	27
#define KEY29	28
#define KEY30	29
#define KEY31	30
#define KEY32	31
/* Analog config
 * Number of analog channel in range of 1-34
 * If not defined or set to zero, analog section are disabled
 * 
 * 1-8 Channel without multiplexer
 * Use: PA0-7
 * 
 * 9-19 Channel with one multiplexer
 * Use: PA0 for multiplexer and PA1-3 for analog signal
 * 
 * 20-34 Channel with two multiplexer
 * Use: PA0,1 for multiplexer and PA2,3 for analog signal
 */
#define ADC_CHANNEL 2
/* #define ADC_MIDI_CHANNEL 1   // Channel number, range 0-15 (channel 1-16), if not define, defualt: 0 */
/* control change number for each analog channel, possible value 0-119, 120-127 are reserved as channel mode message */
#define ADC1	16
#define ADC2	17
#define ADC3	18
#define ADC4	19
#define ADC5	20
#define ADC6	21
#define ADC7	22
#define ADC8	23
#define ADC9	24
#define ADC10	25
#define ADC11	26
#define ADC12	27
#define ADC13	28
#define ADC14	29
#define ADC15	30
#define ADC16	31
/*
 * SPI config
 */
#define SPI_ENABLE
/* misc */
#ifdef INC_ENC
	#if ENC_INC > 16
		#error "INC_ENC out of range, config.h"
	#else
		#if INC_ENC < 1
			#undef INC_ENC
		#else
            #ifndef INC_MIDI_CHANNEL
                #define INC_MIDI_CHANNEL 0
            #else
                #if INC_MIDI_CHANNEL < 0
                #if INC_MIDI_CHANNEL > 15
                    #error "INC_MIDI_CHANNEL out of range, config.h"
                #endif
                #endif
            #endif
			#ifndef INC_MAX
				#define INC_MAX 127
			#else 
                #if INC_MAX < 1
				#if INC_MAX > 127
					#error "INC_MAX out of range, config.h"
				#endif
                #endif
			#endif
			#ifndef INC_STEP
				#define INC_STEP 1
			#else
                #if INC_STEP < 1
				#if INC_STEP > 10
					#error "INC_STEP out of range, config.h"
                #endif
				#endif
			#endif
		#endif
	#endif
#endif
#ifdef KEY
	#if KEY > 32
		#error "KEY out of range, config.h"
	#else
		#if KEY < 1
			#undef KEY
        #else
            #ifndef KEY_MIDI_CHANNEL
                #define KEY_MIDI_CHANNEL 0
            #else
                #if KEY_MIDI_CHANNEL < 0
                #if KEY_MIDI_CHANNEL > 15
                    #error "KEY_MIDI_CHANNEL out of range, config.h"
                #endif
                #endif
            #endif
		#endif
	#endif
#endif
#ifdef ADC_CHANNEL
	#if ADC_CHANNEL > 34
		#error "ADC_CHANNEL out of range, config.h"
	#else
		#if ADC_CHANNEL < 1
			#undef ADC_CHANNEL
		#else
            #ifndef ADC_MIDI_CHANNEL
                #define ADC_MIDI_CHANNEL 0
            #else
                #if ADC_MIDI_CHANNEL < 0
                #if ADC_MIDI_CHANNEL > 15
                    #error "ADC_MIDI_CHANNEL out of range, config.h"
                #endif
                #endif
            #endif
			#if ADC_CHANNEL <= 8
				#define ADC_MODE 1
			#endif
			#if ADC_CHANNEL >= 9 && ADC_CHANNEL <= 19
				#define ADC_MODE 2
			#endif
			#if ADC_CHANNEL >= 20 && ADC_CHANNEL <= 34
				#define ADC_MODE 3
			#endif
		#endif
    #endif
#endif
#endif /* __config_h_included__ */
