/*
 * Name: main.c
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 25.08.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
 #include "main_new.h"

/*
 * Global values
 */
#ifdef INC_ENC_NUMBER
#if INC_ENC_MODE == 0
uchar incEncResult[INC_ENC_NUMBER];
#endif
#endif
/* --------------------------
 * ---------- MAIN ----------
 * ------------------------*/
int main(void) {
	uchar i;
	uchar matrixData[8];

	hardwareInit();
	matrixRead(matrixData);
#ifdef INC_ENC_NUMBER
	uchar prevIncEnc[INC_ENC_STORAGE];		/* Incremental encoder init. */
	for(i = 0; i <= INC_ENC_STORAGE-1; i++)
		prevIncEnc[i] = matrixData[i];		/* set current phases */
#if INC_ENC_MODE == 0
	for(i = 0; i <= INC_ENC_NUMBER-1; i++)
		incEncResult[i] = 0x00;				/* set values to zero */
#endif
#endif
	usbSetup();								/* USB stack init. */
	wdt_enable(WDTO_1S);					/* Watchdog enable(1s) */
    sei();									/* Global interrut enable */
	while(1) {
		wdt_reset();
        usbPoll();
        matrixRead(matrixData);
#ifdef INC_ENC_NUMBER
		incremental_encoder(matrixData, prevIncEnc);
#endif
	}
	return 0;
}
void hardwareInit(void) {
#ifdef INC_ENC_NUMBER
	PORTC = 0x00;
	DDRC = 0xff;
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
/* 
 * Incremental encoder function
 */
#ifdef INC_ENC_NUMBER
/* incremental encoder input table */
#ifdef INC_ENC_MODE == 0
const uchar incEncInputTab[] = {
	0,		// 0000
	0,		// 0001
	1,		// 0010
	0,		// 0011
	0,		// 0100
	0,		// 0101
	0,		// 0110
	127,	// 0111
	127,	// 1000
	0,		// 1001
	0,		// 1010
	0,		// 1011
	0,		// 1100
	1,		// 1101
	0,		// 1110
	0		// 1111
};
#else
const uchar incEncInputTab[] = {
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
#endif
/* incremental encoder midi message table */
const uchar midiIncTab[] = {
	INC_ENC1, INC_ENC2, INC_ENC3, INC_ENC4, INC_ENC5, INC_ENC6, INC_ENC7, INC_ENC8, INC_ENC9, INC_ENC10, INC_ENC11, INC_ENC12, INC_ENC13, INC_ENC14, INC_ENC15, INC_ENC16
};
/*
 * 8 incremental encoder per 2 byte
 * *phase: 		array-pointer on new value, min. 2 byte, 1. byte aPhase, 2. byte bPhase
 * *prevPhase:	array-pointer on old value, min. 2 byte, 1. byte aPhase, 2. byte bPhase
 */
void incremental_encoder(uchar *phase, uchar *prevPhase) {
    uchar tabIndex;                     /* midi table index for more as 8 encoder */
    uchar bitMsk;                       /* for delete overflow-bits */
    uchar coe = INC_ENC_NUMBER;			/* for calculate overflow-bits, number of byte to check in loop */
    uchar byteCnt = 0;                  /* count byte for phase-pointer */
    
    while(coe > 8) {                    /* find byte with overflow bits */
        coe -= 8;
        if(coe > 0)
            byteCnt++;
    }
    if((coe < 8)&& coe) {				/* generate bit mask for valid bits */
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
        incremental_encoder_8bit(phase+byteCnt, prevPhase+byteCnt, tabIndex);
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
 * tabIndex:	set to zero when incTab has only 16 or less values
 */
void incremental_encoder_8bit(uchar *phase, uchar *prevPhase, uchar tabIndex) {
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
			value = incEncInputTab[index];							/* read value from Increment-Table */
			if(value != 0x00) {
#if INC_ENC_MODE > 0
				if(value == 0x01) {
					incEncResult[encNum] += INC_STEP;
					if(incEncResult[encNum] < 0)
						incEncResult[encNum] = INC_MAX;
				} else {
					incEncResult[encNum] -= INC_STEP;
					if(incEncResult[encNum] < 0)
						incEncResult[encNum] = 0;
				}
#endif
				midiMsg[msgLen++] = 0x0b;
				midiMsg[msgLen++] = 0xb0 | INC_MIDI_CHANNEL;
				midiMsg[msgLen++] = midiIncTab[encNum+tabIndex];
#if ENC_ENC_MODE > 0
				midiMsg[msgLen++] = incEncResult[encNum];
#else
				midiMsg[msgLen++] = value;
#endif
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
#endif /* INC_ENC_NUMBER */