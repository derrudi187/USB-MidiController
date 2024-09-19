/*
 * Name: main.c
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 25.08.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
#ifndef __main_h_included__
#define __main_h_included__

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> /* PROGMEM */
#include <avr/wdt.h>
#include <util/delay.h>
/* usbdrv */
#include "usbdrv.h"
#ifdef DEBUG_LEVEL
    #if DEBUG_LEVEL > 0
        #include "oddebug.h"
    #endif
#endif
/* midi-controller */
#include "hardware.h"
#include "config.h"
void hardwareInit(void);
void matrixRead(uchar *data);
void usbSetup(void);
void incremental_encoder(uchar *phase, uchar *prevPhase);
void incremental_encoder_8bit(uchar *phase, uchar *prevPhase, uchar tabIndex);
void key(uchar *key, uchar *prevKey);
void adcInit(short *prevAdcData);
uchar adcConvert(short *prevAdcData, uchar channel);
void usbSendMessage(uchar *message, uchar len);
uchar spiWriteRead(uchar data);
void writeLEDs(uchar *data, uchar len);
#endif /* __main_h_included__ */