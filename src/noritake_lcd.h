/*
 * noritake_lcd.h
 *
 *  Created on: Oct 16, 2013
 *      Author: Mariano
 */

#ifndef NORITAKE_LCD_H_
#define NORITAKE_LCD_H_

//#define ASYNC
#define SYNC
//#define NOPARI
//#define PARI

//pines de enable para la serial y el lcd
#define ENA_SERIAL_LCD GPIOA_PIN0_ON
#define DISA_SERIAL_LCD GPIOA_PIN0_OFF

#define ENA_SERIAL GPIOA_PIN1_ON
#define DISA_SERIAL GPIOA_PIN1_OFF

#define TXD_IN			GPIOA_PIN7_IN
#define TX_LCD_OFF		GPIOA_PIN6_OFF
#define TX_LCD_ON		GPIOA_PIN6_ON
#define TX_SERIE_OFF	GPIOA_PIN5_OFF
#define TX_SERIE_ON		GPIOA_PIN5_ON

#define LED_COMM		GPIOA_PIN4_OUT
#define LED_COMM_ON		GPIOA_PIN4_ON
#define LED_COMM_OFF	GPIOA_PIN4_OFF


/* COMANDOS NORITAKE TABLE 1
BS 08h Cursor moves to the left by one character.
HT 09h Cursor moves to the right by one character.
LF 0Ah Cursor moves to next lower line.
HOM 0Bh Cursor moves to home position (top left).
CR 0Dh Cursor moves to left end of current line.
CLR 0Ch Display screen is cleared and cursor moves to home position.
ESC 1Bh,, Start extended command sequence.
US 1Fh Start extended command sequence.
*/
#define BS	0x08
#define HT	0x09
#define LF	0x0A
#define HOM	0x0B
#define CR	0x0D
#define CLR	0x0C
#define ESC	0x1B
#define US	0x1F

#define OVRWR 0x01
#define VSCROLL 0x02
#define HSCROLL 0x03

#define SCROLL	0x73



/* COMANDOS NORITAKE TABLE 2
Common command table2 Expand command sequences

Initialize 1Bh 40h			Settings return to default values.
							Jumper settings are not re-loaded.
							
Cursor set 1Fh 24h xL xH	Cursor moves to the specified (X, Y) position on
yL yH						Display Memory.
							
Cursor display 1Fh 43h n 	Cursor display setting.
							n = 00h: Cursor display OFF (Default)
							n = 01h: Cursor display ON

Font Width 1Fh 28h 67h		Set font width for 1byte characters.
03h w						w = 00h: 1 dot space on right side
							w = 01h: 1 dot space on right side and left side
							w = 02h: 1 dot space on right side
							w = 03h: 1 dot space on right side and left side
							
Font Magnification 1Fh 28h 67h	Set character magnification ‘x’ times to the right
40h x y							and ‘y’ times downward.

Over-write mode 1Fh 01h			Display mode set to Over-write mode.

Vertical scroll mode 1Fh 02h 	Display mode set to Vertical scroll mode.

Horizontal scroll mode 1Fh 03h 	Display mode set to Horizontal scroll mode.

Horizontal scroll speed 1Fh 73h n 	Set speed for Horizontal scroll mode.
									It takes (T * (n-1)) msec to move one dot.
									n=0 : Fastest, n=1 : T/2 msec. T is approximately
									10~20 msec and changes with item numbers.
									
Reverse display 1Fh 72h n		Reverse display ON/OFF for character and
								image display. Changing this setting only affects
								subsequent data. Content already displayed is
								not affected.
Write mixture display
mode
1Fh 77h n Newly-written characters and images are
combined with current display contents in
Display Memory.
n = 00h: Over write (Default)
n = 01h: OR display write
n = 02h: AND display write
n = 03h: EX-OR display write

Brightness level setting 1Fh 58h n		Set display brightness level.
										Brightness: n/8, n=1~8, n=8: 100%
										
Wait 1Fh 28h 61h						Waits for the specified time.
01h t									(Command and data processing is stopped)
										Wait time = t × approximately 0.5s, n=0~255

Scroll display action 1Fh 28h 61h		Shift the display screen.
10h wL wH cL							It becomes a scroll action by setting up two or more shifts.
cH s n

Blink 1Fh 28h 61h 11h					Blink display action. Blink pattern specified by
p t1 t2 c								‘p’.

Screen saver 1Fh 28h 61h				Control Power ON or OFF, and Start Screen
40h p									saver mode. This Screen saver mode setting is
										cancelled when next data is written.
										p = 00h: Display power OFF (Power save mode)
										p = 01h: Display power ON
										p = 02h: All dot OFF
										p = 03h: All dot ON
										p = 04h: Repeat blink display with normal and Reverse display
										
*/

//funciones del modulo
void LCD_Init(void);
void LCD_Str(unsigned char *);
void LCD_Command(unsigned char);
void LCD_Ext_Command(unsigned char *);






#endif /* NORITAKE_LCD_H_ */
