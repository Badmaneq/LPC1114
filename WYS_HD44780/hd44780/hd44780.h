/*
 * hd44780.h
 *
 *  Created on: 27 wrz 2015
 *      Author: badman
 */

#ifndef HD44780_H_
#define HD44780_H_

#include "LPC11xx.h"

//----------------------------------------------------------------------------------------
//
//		Parametry pracy sterownika
//
//----------------------------------------------------------------------------------------
// rozdzielczosc wyswietlacza LCD (wiersze/kolumny)
#define LCD_Y 2		// ilosc wierszy wyswietlacza LCD
#define LCD_X 16	// ilosc kolumn wyswietlacza LCD

// tu ustalamy za pomoca zera lub jedynki czy sterujemy pinem RW
//	0 - pin RW podlaczony na stale do GND
//	1 - pin RW podlaczony do mikrokontrolera
#define USE_RW 0

//----------------------------------------------------------------------------------------
//
//		Ustawienia sprzetowe polaczen sterownika z mikrokontrolerem
//
//----------------------------------------------------------------------------------------
// tu konfigurujemy port i piny do jakich podlaczymy linie D7..D4 LCD
#define LCD_D7PORT  3
#define LCD_D7 5
#define LCD_D6PORT  3
#define LCD_D6 4
#define LCD_D5PORT  3
#define LCD_D5 3
#define LCD_D4PORT  3
#define LCD_D4 2


// tu definiujemy piny procesora do ktorych podlczamy sygnaly RS,RW, E
#define LCD_RSPORT 3
#define LCD_RS 0

#define LCD_RWPORT 0
#define LCD_RW 1

#define LCD_EPORT 3
#define LCD_E 1
//------------------------------------------------  koniec ustawien sprzetowych ---------------

//----------------------------------------------------------------------------------------
//****************************************************************************************
//*																						 *
//*		U S T A W I E N I A   KOMPILACJI												 *
//*																						 *
//*		Wlaczamy kompilacje komend uzywanych lub wylaczamy nieuzywanych					 *
//*		(dzieki temu regulujemy zajetosc pamieci FLASH po kompilacji)					 *
//*																						 *
//*		1 - oznacza WLACZENIE do kompilacji												 *
//*		0 - oznacza wylaczenie z kompilacji (funkcja niedostepna)						 *
//*																						 *
//****************************************************************************************
//----------------------------------------------------------------------------------------

#define USE_LCD_LOCATE	1			// ustawia kursor na wybranej pozycji Y,X (Y=0-3, X=0-n)

#define USE_LCD_CHAR 	1			// wysyla pojedynczy znak jako argument funkcji


#define USE_LCD_INT 	1			// wyswietla liczbe dziesietna na LCD
#define USE_LCD_HEX 	0			// wyswietla liczbe szesnastkowa na LCD

#define USE_LCD_DEFCHAR		0		// wysyla zdefiniowany znak z pamieci RAM

#define USE_LCD_CURSOR_ON 		0	// obsluga wlaczania/wylczania kursora
#define USE_LCD_CURSOR_BLINK 	0	// obsluga wlaczania/wylaczania migania kursora
#define USE_LCD_CURSOR_HOME 	0	// ustawia kursor na pozycji poczatkowej

//------------------------------------------------  koniec ustawien kompilacji ---------------

// definicje adresow w DDRAM dla roznych wyswietlaczy
// inne saw wyswietlaczach 2wierszowych i w 4wierszowych
#if ( (LCD_Y == 4) && (LCD_X == 16) )
#define LCD_LINE1 0x00		// adres 1 znaku 1 wiersza
#define LCD_LINE2 0x28		// adres 1 znaku 2 wiersza
#define LCD_LINE3 0x14  	// adres 1 znaku 3 wiersza
#define LCD_LINE4 0x54  	// adres 1 znaku 4 wiersza
#else
#define LCD_LINE1 0x00		// adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40		// adres 1 znaku 2 wiersza
#define LCD_LINE3 0x10  	// adres 1 znaku 3 wiersza
#define LCD_LINE4 0x50  	// adres 1 znaku 4 wiersza
#endif


// Komendy sterujace
#define LCDC_CLS					0x01
#define LCDC_HOME					0x02
#define LCDC_ENTRY					0x04
	#define LCDC_ENTRYR					0x02
	#define LCDC_ENTRYL					0
	#define LCDC_MOVE					0x01
#define LCDC_ONOFF					0x08
	#define LCDC_DISPLAYON				0x04
	#define LCDC_CURSORON				0x02
	#define LCDC_CURSOROFF				0
	#define LCDC_BLINKON				0x01
#define LCDC_SHIFT					0x10
	#define LCDC_SHIFTDISP				0x08
	#define LCDC_SHIFTR					0x04
	#define LCDC_SHIFTL					0
#define LCDC_FUNC					0x20
	#define LCDC_FUNC8B					0x10
	#define LCDC_FUNC4B					0
	#define LCDC_FUNC2L					0x08
	#define LCDC_FUNC1L					0
	#define LCDC_FUNC5x10				0x04
	#define LCDC_FUNC5x7				0
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80


// deklaracje funkcji na potrzeby innych modulow
void lcd_init(void);								// WLACZONA na stale do kompilacji
void lcd_cls(void);									// WLACZONA na stale do kompilacji
void lcd_str(char *str);							// WLACZONA na stale do kompilacji
void lcd_locate(uint8_t y, uint8_t x);
void lcd_char(char c);
void lcd_int(int val);
void lcd_hex(int val);
void lcd_defchar(uint8_t nr, uint8_t *def_znak);
void lcd_home(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_blink_on(void);
void lcd_blink_off(void);

#endif /* HD44780_H_ */
