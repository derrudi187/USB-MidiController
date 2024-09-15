/*
 * Name: hardware.h
 * Project: USB-MidiController
 * Author: DerRudi
 * Creation Date: 03.09.2024
 * Copyright: (c) 2024 by DerRudi
 * License: GNU GPL v3
 */
#ifndef __hardware_h_included__
#define __hardware_h_included__
/*
							PORT			DDR					Description         Alternative
PORTA
PA0 = ADC Channel 0			0				0                   Analog 1            Analog 1
PA1 = ADC Channel 1			0				0                   Analog 2            Analog 2
PA2 = ADC Channel 2			0				0                   Analog 3            Analog 3
PA3 = ADC Channel 3			0				0                   Analog 4            Analog 4
PA4 = ADC Channel 4	        0				1                   Analog 5            Multiplexer Select 0
PA5 = ADC Channel 5		    0				1                   Analog 6            Multiplexer Select 1
PA6 = ADC Channel 6		    0				1                   Analog 7            Multiplexer Select 2
PA7 = ADC Channel 7		    0				1                   Analog 8            Multiplexer Select 3

PORTB
PB0	= In bit0      			0				0                   Matrix in
PB1	= In bit1	        	0				0                   ..
PB2	= In bit2		        0				0                   ..
PB3 = In bit3		        0				0                   ..
PB4	= LATCH					0				1                   Latch out(74HCT573)
PB5	= MOSI					0				1                   SPI data out
PB6 = MISO					1				0                   SPI data in
PB7	= SCK					0				1                   SPI sck

PORTC
PC0 = Out bit0				0				1                   Matrix, latch register out
PC1 = Out bit1				0				1                   ..
PC2 = Out bit2				0				1                   ..
PC3 = Out bit3				0				1                   ..
PC4 = Out bit4				0				1                   ..
PC5 = Out bit5				0				1                   ..
PC6 = Out bit6				0				1                   ..
PC7 = Out bit7				0				1                   ..

PORTD
PD0 = In bit4   			0				0                   Matrix in
PD1 = TXD					0				1                   USART transmit
PD2 = USB D+				0				0                   USB D+ (usbconfig.h)
PD3 = In bit5   			0				0                   Matrix in
PD4 = In bit6	    		0				0                   ..
PD5 = In bit7		    	0				0                   ..
PD6 = USB D-				0				0                   USB D- (usbconfig.h)
PD7 = USB PullUp			0				0                   USB connect/disconnect (usbconfig.h) */

#define MLPX_OUT    PORTA    /* Multiplexer output */
#define ANA_CS0     4
#define ANA_CS1     5
#define ANA_CS2     6
#define ANA_CS3     7

#define MXIN_LN     PINB    /* Matrix in, low nibble */
#define MXIN0       0
#define MXIN1       1
#define MXIN2       2
#define MXIN3       3
#define BUS_LATCH	4
#define SPI_MOSI	5
#define SPI_MISO	6
#define SPI_SCK	    7

#define MXOUT       PORTC   /* Matrix, latch register out */
#define MXOUT0      0
#define MXOUT1      1
#define MXOUT2      2
#define MXOUT3      3
#define MXOUT4      4
#define MXOUT5      5
#define MXOUT6      6
#define MXOUT7      7

#define MXIN_HN     PIND    /* Matrix in, high nibble */
#define MXIN4       0
#define USART_TX	1
/* USB D+ */
#define MXIN5       3
#define MXIN6       4
#define MXIN7       5
/* USB D- */
/* USB pullup */
#endif // __hardware_h_included__