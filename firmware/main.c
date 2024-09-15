/*
 * Name: main.c
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 25.08.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
#include "main.h"
/* 
 * USB device and configuration descriptor
 */
#define USBDESCR_CS_INTERFACE 0x24  /* Class-specific Interface */
#define USBDESCR_CS_ENDPOINT 0x25	/* Class-specific Endpoint  */
#define MIDI_IN_JACK 0x02			/* Midi-In Jack             */
#define MIDI_OUT_JACK 0x03			/* Midi-Out Jack            */
/* 
 * Appendix B. Example1: Simple MIDI Instrument (Informative)
 */
// B.1.1 Device Descriptor
static PROGMEM const char usbDescriptorDeviceMidi[] = {
	0x12,													// bLength
	USBDESCR_DEVICE,										// bDescriptorType
	0x10, 0x01,												// bcdUSB
	USB_CFG_DEVICE_CLASS,									// bDeviceClass
	USB_CFG_DEVICE_SUBCLASS,								// bDeviceSubClass
	0x00,													// bDeviceProtocol
	0x08,													// bMAxPacketSize0
	(char)USB_CFG_VENDOR_ID,								// idVendor
    (char)USB_CFG_DEVICE_ID,								// idProduct
	USB_CFG_DEVICE_VERSION,									// Device Release Code
	1,														// iManufacturer
	2,														// iProduct
	0,														// iSerialNumber
	0x01													// bNumConfigurations
};
// B.1.2 Configuration Descriptor
static PROGMEM const char usbDescriptorConfigurationMidi[] = {
	0x09,													// bLength
	USBDESCR_CONFIG,										// bDescriptorType
	0x65, 0x00,												// wTotalLength (sizeof usbDescriptorConfiguration)
	0x02,													// bNumInterfaces (Audio Control & MIDI Streaming Interface)
	0x01,													// bConfigurationValue
	0x00,													// iConfiguration
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       					// bmAttributes (0x80 | 0x40)
#else
    (1 << 7),                           					// bmAttributes (0x80)
#endif
    USB_CFG_MAX_BUS_POWER / 2,            					// MaxPower
/*
 * AudioControl Interface Descriptor
 */
// B.2.1 Standart AC
	0x09,													// bLength
	USBDESCR_INTERFACE,										// bDescriptorType
	0x00,													// bInterfaceNumber
	0x00,													// bAlternateSetting
	0x00,													// bNumEndpoints
	0x01,													// bInterfaceClass
	0x01,													// bInterfaceSubClass
	0x00,													// bInterfaceProtocol
	0x00,													// iInterface
// B.2.2 Class-specific AC
	0x09,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	0x01,													// bDescriptorSubtype
	0x00, 0x01,												// bcdADC
	0x09, 0x00,												// wTotalLength
	0x01,													// bInCollection
	0x01,													// baInterfaceNr(1)
/*
 * MIDIStreaming Interface Descriptor
 */
// B.3.1 Standart
	0x09,													// bLength
	USBDESCR_INTERFACE,										// bDescriptorType
	0x01,													// bInterfaceNumber
	0x00,													// bAlternateSetting
	0x02,													// bNumEndpoints
	0x01,													// bInterfaceClass (Audio)
	0x03,													// bInterfaceSubClass (Midistreaming)
	0x00,												 	// bInterfaceProtocol
	0x00,													// bInterface
// B.3.2 Class-specific
	0x07,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	0x01,													// bDescriptorSubtype
	0x00, 0x01,												// bcdMSC
	0x41, 0x00,												// wTotalLength (sizeof at B.3.2)
/*
 * MIDI IN Jack Descriptor
 */
// B.3.3 MIDI IN (embedded)
	0x06,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	MIDI_IN_JACK,											// bDescriptorSubtype
	0x01,													// bJackType (embedded)
	0x01,													// bJackID
	0x00,													// iJack
// B.3.4 MIDI IN (external)
	0x06,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	MIDI_IN_JACK,											// bDescriptorSubtype
	0x02,													// bJackType (external)
	0x02,													// bJackID
	0x00,													// iJack
/*
 * MIDI OUT Jack Descriptor
 */
// B.3.5 MIDI OUT (embedded)
	0x09,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	MIDI_OUT_JACK,											// bDescriptorSubtype
	0x01,													// bJackType (embedded)
	0x03,													// bJackID
	0x01,													// bNrInputPins
	0x02,													// BaSourceID(1)
	0x01,													// BaSourcePin(1)
	0x00,													// iJack
// B.3.6 MIDI OUT (external)
	0x09,													// bLength
	USBDESCR_CS_INTERFACE,									// bDescriptorType
	MIDI_OUT_JACK,											// bDescriptorSubtype
	0x02,													// bJackType (external)
	0x04,													// bJackID
	0x01,													// bNrInputPins
	0x01,													// BaSourceID(1)
	0x01,													// BaSourcePin(1)
	0x00,													// iJack
/*
 * Bulk OUT Endpoint Descriptor
 */
// B.4.1 Standart
	0x09,													// bLength
	USBDESCR_ENDPOINT,										// bDescriptorType
	0x01,													// bEndpointAddress
	0x03,													// bmAttributes (2 = Bulk, 3 = Interrupt)
	0x08, 0x00,												// wMaxPacketSize
	0x0A,													// bInterval
	0x00,													// bRefresh
	0x00,													// bSynchAddress
// B.4.2 Class-specific MIDIStream
	0x05,													// bLength
	USBDESCR_CS_ENDPOINT,									// bDescriptorType
	0x01,													// bDescriptorSubtype
	0x01,													// bNumEmbMIDIJack
	0x01,													// BaAssocJackID(1)
/*
 * Bulk IN Endpoint Descriptor
 */
// B.5.1 Standart
	0x09,													// bLength
	USBDESCR_ENDPOINT,										// bDescriptorType
	0x81,													// bEndpointAddress
	0x03,													// bmAttributes (2 = Bulk, 3 = Interrupt)
	0x08, 0x00,												// wMaxPacketSize
	0x0A,													// bInterval
	0x00,													// bRefresh
	0x00,													// bSynchAddress
// B.5.2 Class-specific MIDIStream
	0x05,													// bLength
	USBDESCR_CS_ENDPOINT,									// bDescriptorType
	0x01,													// bDescriptorSubtype
	0x01,													// bNumEmbMIDIJack
	0x03													// BaAssocJackID(1)
};
/*
 * Global SPI data
 */
#ifdef SPI_ENABLE
uchar spiDataOut[SPI_DATA];
#endif
/* -----------------------------------------------------------
 * ---------- Request from Host to setup the device ----------
 * -------------------------------------------------------- */
uchar usbFunctionDescriptor(usbRequest_t * rq) {
	if(rq->wValue.bytes[1] == USBDESCR_DEVICE) {
		usbMsgPtr = (unsigned short) usbDescriptorDeviceMidi;
		return sizeof(usbDescriptorDeviceMidi);
	} else {
		usbMsgPtr = (unsigned short) usbDescriptorConfigurationMidi;
		return sizeof(usbDescriptorConfigurationMidi);
	}
}
/* -----------------------------------
 * ---------- USB Interface ----------
 * -------------------------------- */
uchar usbFunctionSetup(uchar data[8]) {
	// debug LED
	//PORTC ^= 0x01;
	return USB_NO_MSG;
}
/* usbFunctionWriteOut */
void usbFunctionWriteOut(uchar *data, uchar len) {
	// debug LED
	// PORTC ^= 0x20;
#ifdef DEBUG_LEVEL
	#if DEBUG_LEVEL > 0
	uchar i;
	for(i = 0; i < len; i++) {
		DBG1(0x01, &data[i], 1);
	}
	/* Data send: 0x81, 0x01, 0x7f
	 * 
	 * received data:
	 * byte0:	0x08	cable number, note off
	 * byte1:	0x81	note off, channel number
	 * byte2:	0x01	note number
	 * byte3:	0x7f	velocity
	 * 
	 * Data send: 0x91, 0x00, 0x7f
	 * 
	 * received data
	 * byte0:	0x09
	 * byte1:	0x91
	 * byte2:	0x00
	 * byte3:	0x7f
	 */
	#else
	#ifdef SPI_ENABLE
		writeLEDs(data, len);
	#endif
	#endif
#endif

	return;
}
/* --------------------------
 * ---------- MAIN ----------
 * ------------------------*/
int main(void) {
    uchar i;
    uchar matrixData[8];
    
    hardwareInit();
    matrixRead(matrixData);
#ifdef INC_ENC
    uchar prevInc[4];
    char incValue[INC_ENC];
    for(i = 0; i <= 3; i++)
        prevInc[i] = matrixData[i];
#endif
#ifdef KEY
    uchar prevKey[4];
    for(i = 0; i <= 3; i++)
        prevKey[i] = matrixData[i+4];
#endif
#ifdef INC_ENC
    for(i = 0; i <= INC_ENC-1; i++)
        incValue[i] = 0x00;
#endif
#ifdef ADC_CHANNEL
    short prevAdcData[ADC_CHANNEL];
    uchar adcChannel = 0;
    adcInit(prevAdcData);
#endif
#ifdef SPI_ENABLE
	for(i = 0; i < SPI_DATA; i++)
		spiDataOut[i] = 0x00;
#endif

    usbSetup();             /* USB setup */
    wdt_enable(WDTO_1S);    /* Watchdog enable(1s) */
    sei();                  /* global interrut enable */
	
    while(1) {
        wdt_reset();
        usbPoll();
        matrixRead(matrixData);
#ifdef INC_ENC
        incremental_encoder(matrixData, prevInc, incValue); 
#endif
#ifdef KEY
        key(matrixData+4, prevKey);
#endif
#ifdef ADC_CHANNEL
        adcChannel = adcConvert(prevAdcData, adcChannel);
#endif
    }
    return 0x00;
}



/* ------------------------------------
 * ---------- Hardware init. ----------
 * ----------------------------------*/
void hardwareInit(void) {
#ifdef ADC_CHANNEL
	#if ADC_CHANNEL > 8
		PORTA = 0x00;
		DDRA = (uchar) (1<<ANA_CS3)|(1<<ANA_CS2)|(1<<ANA_CS1)|(1<<ANA_CS0);
	#endif
#endif
#ifdef SPI_ENABLE
	PORTB = (uchar) (1<<SPI_MISO);
    DDRB =	(uchar) (1<<SPI_SCK)|(1<<SPI_MOSI)|(1<<BUS_LATCH);
	SPCR = (uchar) (1<<SPE)|(1<<MSTR);										/* SPI init. */
	SPSR = (uchar) (1<<SPI2X);
#endif
/* #################################### */
	PORTC = 0x00;
	DDRC = 0xff;
/* #################################### */	
#ifdef DEBUG_LEVEL
	#if DEBUG_LEVEL > 0
		PORTD = 0x00;
		DDRD = (uchar) (1<<USART_TX);
		odDebugInit();														/* USART Init. */
	#endif
#endif
	return;
}
/* Matrix read 8x8 bit
 * *data: array-pointer for data
 */
void matrixRead(uchar *data) {
	uchar i;																    /* byte count */
	uchar bitMsk = 0x01;
	uchar wr;																    /* work register */
	for(i = 0; i <= 7; i++) {
		PORTC = bitMsk;														    /* set active bit */
		_delay_us(1);
		data[i] = (MXIN_LN & ((1<<MXIN3)|(1<<MXIN2)|(1<<MXIN1)|(1<<MXIN0)));    /* read bit 0-3 */
		wr = (MXIN_HN & (1<<MXIN4));											/* read bit 0 */
		wr <<= 0x04;
		data[i] |= wr;
		wr = (MXIN_HN & ((1<<MXIN7)|(1<<MXIN6)|(1<<MXIN5)));					/* read bit 3-5 */
		wr <<= 0x02;
		data[i] |= wr;
		bitMsk <<= 1;														    /* next byte */
	}
	PORTC = 0x00;
	return;
}
/* USB setup */
void usbSetup(void) {
	uchar i;
	usbDeviceDisconnect();
	i = 0;
	while(i--) {
		_delay_ms(1);
	}
	usbDeviceConnect();
	usbInit();
	return;
}



#ifdef INC_ENC
/* incremental encoder table */
const uchar incTab[] = {
	0,		// 0000
	0,		// 0001
	1,		// 0010
	0,		// 0011
	0,		// 0100
	0,		// 0101
	0,		// 0110
	-1,		// 0111
	-1,		// 1000
	0,		// 1001
	0,		// 1010
	0,		// 1011
	0,		// 1100
	1,		// 1101
	0,		// 1110
	0		// 1111
};
/* midi message incremental table */
const uchar midiIncTab[] = {
	INC_ENC1, INC_ENC2, INC_ENC3, INC_ENC4, INC_ENC5, INC_ENC6, INC_ENC7, INC_ENC8, INC_ENC9, INC_ENC10, INC_ENC11, INC_ENC12, INC_ENC13, INC_ENC14, INC_ENC15, INC_ENC16
};
/*
 * 8 incremental encoder per 2 byte
 * *phase: 		array-pointer on new value, min. 2 byte, 1. byte aPhase, 2. byte bPhase
 * *prevPhase:	array-pointer on old value, min. 2 byte, 1. byte aPhase, 2. byte bPhase
 * *result:		array-pointer on result
 * coe:			count of encoder, min. 1 encoder(first 2 bit of byte 1. & 2.)
 */
void incremental_encoder(uchar *phase, uchar *prevPhase, char *result) {
    uchar tabIndex;                     /* midi table index for more as 8 encoder */
    uchar bitMsk;                       /* for delete overflow-bits */
    uchar coe = INC_ENC;                /* for calculate overflow-bits, number of byte to check in loop */
    uchar byteCnt = 0;                  /* count byte for phase-pointer */
    
    while(coe > 8) {                    /* find byte with overflow bits */
        coe -= 8;
        if(coe > 0)
            byteCnt++;
    }
    if((coe < 8)&& coe) {                /* generate bit mask for valid bits */
        bitMsk = 0x00;
        while(coe--) {
            bitMsk <<= 1;
            bitMsk |= 0x01;
        }
        phase[byteCnt] &= bitMsk;       /* delete overflow-bits */
        phase[byteCnt+1] &= bitMsk;
    }
    coe = INC_ENC;
    tabIndex = 0;
    byteCnt = 0;
    while(coe) {
        incremental_encoder_8bit(phase+byteCnt, prevPhase+byteCnt, result, tabIndex);
        if(coe > 8) {
            coe -= 8;
            tabIndex += 8;
            byteCnt += 2;
        } else {
            coe -= coe;
        }
    }
    return;
}
/*
 * 8 incremental encoder per 2 byte
 * *phase: 		array-pointer on new value, 2 byte, 1. byte aPhase, 2. byte bPhase
 * *prevPhase:	array-pointer on old value, 2 byte, 1. byte aPhase, 2. byte bPhase
 * *result:		array-pointer on result
 * tabIndex:	set to zero when incTab has only 16 or less values
 */
void incremental_encoder_8bit(uchar *phase, uchar *prevPhase, char *result, uchar tabIndex) {
	uchar bitMsk = 0x01;
	uchar index;
	uchar value;
	uchar encNum = 0;
	
	uchar midiMsg[8];
	uchar msgLen = 0;
	
	while((phase[0] != prevPhase[0]) || (phase[1] != prevPhase[1])) {
		if(((phase[0] & bitMsk) != (prevPhase[0] & bitMsk)) || ((phase[1] & bitMsk) != (prevPhase[1] & bitMsk))) {
			index = 0x00;
			if(phase[0] & bitMsk)
				index = 0x01;
			if(phase[1] & bitMsk)
				index |= 0x02;
			if(prevPhase[0] & bitMsk)
				index |= 0x04;
			if(prevPhase[1] & bitMsk)
				index |= 0x08;
			value = incTab[index];							/* read value from Increment-Table */
			if(value != 0x00) {
				if(value == 0x01) {
					/*result[encNum] += INC_STEP;
					if(result[encNum] < 0)
						result[encNum] = INC_MAX;
					*/
					result[encNum] = 0x01;
				} else {
					/*
					result[encNum] -= INC_STEP;
					if(result[encNum] < 0)
						result[encNum] = 0;
					*/
					result[encNum] =0x7f;
				}
				midiMsg[msgLen++] = 0x0b;
				midiMsg[msgLen++] = 0xb0 | INC_MIDI_CHANNEL;
				midiMsg[msgLen++] = midiIncTab[encNum+tabIndex];
				midiMsg[msgLen++] = result[encNum];
				if(msgLen == 8) {
					usbSendMessage(midiMsg, msgLen);		/* send message */
					msgLen = 0;
				}
			}
			prevPhase[0] &= ~bitMsk;
			prevPhase[1] &= ~bitMsk;
			prevPhase[0] |= (phase[0] & bitMsk);
			prevPhase[1] |= (phase[1] & bitMsk);
		}
		bitMsk <<= 1;
		encNum++;
	}
	if(msgLen > 0)
		usbSendMessage(midiMsg, msgLen);					/* send message */
	return;
}
#endif /* INC_ENC */
/* 8 Keys per byte
 * *key:		array-pointer on new value
 * *prevKey:	array-pointer on old value
 * nok:			count of keys, min. 1 key
 */
#ifdef KEY
const uchar midiKeyTab[] = {
KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15, KEY16, KEY17, KEY18, KEY19, KEY20, KEY21, KEY22, KEY23, KEY24, KEY25, KEY26, KEY27, KEY28, KEY29, KEY30, KEY31, KEY32
};
void key(uchar *key, uchar *prevKey) {
	uchar byteCnt = KEY;				/* for calculate number of byte to check, for-loop count */
	uchar nok = 0;						/* number of byte to check in for-loop */
	uchar bitMsk;						/* for delete overflow-bits, key check */
	
	uchar keyNum;						/* key number for midiTable */
	uchar tabIndex;						/* midi table index for more as 8 keys */
	
	uchar midiMsg[8];
	uchar msgLen = 0;
			
	while(byteCnt > 8) {				/* find byte with overflow bits and generate byte count for compare */
		byteCnt -= 8;
		if(byteCnt > 0)
			nok++;
	}
	if((byteCnt < 8) && byteCnt) {		/* generate bit mask for valid bits */
		bitMsk = 0x00;
		while(byteCnt--) {
			bitMsk <<= 1;
			bitMsk |= 0x01;
		}
		key[nok] &= bitMsk;				/* delete overflow bits */
	}
	bitMsk = 0x01;
	keyNum = 0;
	tabIndex = 0;
	for(byteCnt = 0; byteCnt <= nok; byteCnt++) {
		while(key[byteCnt] != prevKey[byteCnt]) {
			if((key[byteCnt] & bitMsk) != (prevKey[byteCnt] & bitMsk)) {
				if(key[byteCnt] & bitMsk) {
					prevKey[byteCnt] |= bitMsk;
					midiMsg[msgLen++] = 0x09;
					midiMsg[msgLen++] = 0x90 | KEY_MIDI_CHANNEL;
					midiMsg[msgLen++] = midiKeyTab[keyNum+tabIndex];
					midiMsg[msgLen++] = 0x7f;
                } else {
					prevKey[byteCnt] &= ~bitMsk;
					midiMsg[msgLen++] = 0x08;
					midiMsg[msgLen++] = 0x80 | KEY_MIDI_CHANNEL;
					midiMsg[msgLen++] = midiKeyTab[keyNum+tabIndex];
					midiMsg[msgLen++] = 0x00;
				}
			}
			bitMsk <<= 1;
			keyNum++;
			if(msgLen == 8) {
				// send message
				usbSendMessage(midiMsg, msgLen);
				msgLen = 0;
			}
		}
		bitMsk = 0x01;
		tabIndex += 8;
	}
	if(msgLen)
	// send message
	usbSendMessage(midiMsg, msgLen);
	return;
}
#endif /* KEY */
/* ADC Init.
 * *prevAdcData array-pointer for convert value
 */
#ifdef ADC_CHANNEL
const uchar midiAdcTab[] = {
	ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, ADC10, ADC11, ADC12, ADC13, ADC14, ADC15, ADC16
};
void adcInit(short *prevAdcData) {
	uchar channel;						/* channel count 1-34 */
	uchar adcChannel = 0x00;			/* admux channel count 0x00 - 0x03 */
	
	ADMUX = (uchar) (1<<REFS0);										/* AVCC with external capacitor at AREF pin */
	ADCSRA = (uchar) (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1);		/* First start, prescaler 64 */
	
	for(channel = 0; channel <= ADC_CHANNEL-1; channel++) {
		while(ADCSRA & (1<<ADSC)) {
			; /* wait */
		}
		prevAdcData[channel] = ADC;
#if ADC_MODE == 1
		if(channel == ADC_CHANNEL-1) {	/* for 1-8 adc-channel */
			adcChannel = 0x00;
		} else {
			adcChannel++;
		}
#endif
#if ADC_MODE == 2
		if(channel == ADC_CHANNEL-1) {	/* for 1-19 adc-channel */
			adcChannel = 0x00;
			PORTA &= 0x0f;
		} else {
			if(channel >= 15) {
				adcChannel++;
			} else {
				PORTA += 0x10;
			}
		}
#endif
#if ADC_MODE == 3
		if(channel == ADC_CHANNEL-1) { /* for 1-34 adc-channel */
			adcChannel = 0x00;
			PORTA &= 0x0f;
		} else {
			if(adcChannel == 0x01) {
				if(channel >= 31) {
					adcChannel = 0x02;
				} else {
					adcChannel = 0x00;
					PORTA += 0x10;
				}
			} else {
				adcChannel++;
			}
		}
#endif
		ADMUX = (uchar) (adcChannel | (1<<REFS0));					/* set next channel */
		ADCSRA = (uchar) (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1);	/* start for next channel */
	}
	return;
}
/* ADC convert
 * *prevAdcData: array-pinter for convert value
 * channel: next channel (count, start with zero)
 * return: value of next channel
 */
uchar adcConvert(short *prevAdcData, uchar channel) {
	short adcData;
	uchar adcChannel;
	
	uchar midiMsg[4];
	
	if(!(ADCSRA & (1<<ADSC))) {
		adcData = ADC;
		if(prevAdcData[channel] - adcData > 7 || prevAdcData[channel] - adcData < -7) {
			prevAdcData[channel] = adcData;
			midiMsg[0] = 0x0b;
			midiMsg[1] = 0xb0 | ADC_MIDI_CHANNEL;
			midiMsg[2] = midiAdcTab[channel];
			midiMsg[3] = adcData >> 3;
			// send message
			usbSendMessage(midiMsg, 4);
		}
		adcChannel = ADMUX & 0x07;
#if ADC_MODE == 1
		if(channel == ADC_CHANNEL-1) {	/* for 1-8 adc-channel */
			channel = 0;
			adcChannel = 0x00;
		} else {
			channel++;
			adcChannel++;
		}
#endif
#if ADC_MODE == 2
		if(channel == ADC_CHANNEL-1) {	/* for 1-19 adc-channel */
			channel = 0;
			adcChannel = 0x00;
			PORTA &= 0x0f;
		} else {
			if(channel >= 15) {
				adcChannel++
			} else {
				PORTA += 0x10;
			}
			channel++;
		}
#endif
#if ADC_MODE == 3
		if(channel == ADC_CHANNEL-1) {	/* for 1-34 adc-channel */
			channel = 0;
			adcChannel = 0x00;
			PORTA &= 0x0f;
		} else {
			if(adcChannel == 0x01) {
				if(channel >= 31) {
					adcChannel = 0x02;
				} else {
					adcChannel = 0x00;
					PORTA += 0x10;
				}
			} else {
				adcChannel++;
			}
			channel++;
		}
#endif
		ADMUX = (uchar) adcChannel | (1<<REFS0);					/* set next channel */
		ADCSRA = (uchar) (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1);	/* start for next channel */
	}
	return channel;
}
#endif /* ADC_CHANNEL */
void usbSendMessage(uchar *message, uchar len) {
	if(usbInterruptIsReady()) {
		usbSetInterrupt(message, len);
	}
	return;
}
uchar spiWriteRead(uchar data) {
	SPDR = data;				/* SPI write */
	while(!(SPSR & (1<<SPIF))) {
		; /* wait SPI ready */
	}
	return SPDR;				/* SPI read(clear SPIF bit) */
}
void writeLEDs(uchar *data, uchar len) {
	uchar byteCnt = 0;
	uchar byteMsk = 0x01;
	uchar bitMsk = 0x01;
	
	if(len == 4) {
		if(data[0] == 0x09) {								/* check cable number, code index number */		
			if((data[1] & 0xf0) == 0x90) {					/* check note on */
				if((data[1] & 0x0f) == LED_CHANNEL) {		/* check channel */
					if(data[2] <= LED-1) {
						while(data[2] >= 8) {
							data[2] -= 8;
							byteCnt++;
							byteMsk <<= 1;					/* SPI latch bit */
						}
						while(data[2]--) {
							bitMsk <<= 1;					/* LED bit, set or clear */
						}
						if(data[3] > 0x00 && data[3] <= 0x7f) {
							spiDataOut[byteCnt] |= bitMsk;	/* set bit */
						} else {
							spiDataOut[byteCnt] &= ~bitMsk;/* clear bit */
						}
						spiWriteRead(spiDataOut[byteCnt]);
						byteCnt = PORTC;					/* save PORTC status */
						PORTC = byteMsk;					/* write bit to latch */
						PORTB |= (1<<BUS_LATCH);
						PORTC = 0x00;
						PORTB &= ~(1<<BUS_LATCH);
						PORTC = byteCnt;					/* recover PORTC status */
					}
				}
			}
		}
	}
	return;
}
