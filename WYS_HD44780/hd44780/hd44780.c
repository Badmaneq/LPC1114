/*
 * hd44780.c
 *
 *  Created on: 27 wrz 2015
 *      Author: badman
 */

#include "hd44780.h"
#include "delay.h"


// Makra upraszczajace dostep do portow
#define LPC_GPIO(x) SPORT(x)
#define SPORT(x) (LPC_GPIO##x)

#define SET_RS 	LPC_GPIO(LCD_RSPORT)->DATA |= (1<<LCD_RS)	// stan wysoki na linii RS
#define CLR_RS 	LPC_GPIO(LCD_RSPORT)->DATA &= ~(1<<LCD_RS)	// stan niski na linii RS

#define SET_RW 	LPC_GPIO(LCD_RWPORT)->DATA |= (1<<LCD_RW)	// stan wysoki na RW - odczyt z LCD
#define CLR_RW 	LPC_GPIO(LCD_RWPORT)->DATA &= ~(1<<LCD_RW)	// stan niski na RW - zapis do LCD

#define SET_E 	LPC_GPIO(LCD_EPORT)->DATA |= (1<<LCD_E)		// stan wysoki na linii E
#define CLR_E 	LPC_GPIO(LCD_EPORT)->DATA &= ~(1<<LCD_E)	// stan niski na linii E

// ustawia piny jako wyjsciowe
static inline void data_dir_out(void)
{
	LPC_GPIO(LCD_D7PORT)->DIR |= (1<<LCD_D7);
	LPC_GPIO(LCD_D6PORT)->DIR |= (1<<LCD_D6);
	LPC_GPIO(LCD_D5PORT)->DIR |= (1<<LCD_D5);
	LPC_GPIO(LCD_D4PORT)->DIR |= (1<<LCD_D4);
}

// ustawia piny jako wejsciowe
static inline void data_dir_in(void)
{
	LPC_GPIO(LCD_D7PORT)->DIR &= (1<<LCD_D7);
	LPC_GPIO(LCD_D6PORT)->DIR &= (1<<LCD_D6);
	LPC_GPIO(LCD_D5PORT)->DIR &= (1<<LCD_D5);
	LPC_GPIO(LCD_D4PORT)->DIR &= (1<<LCD_D4);
}

// wysyla pol bajtu D4...D7
static inline void lcd_sendHalf(uint8_t data)
{
	if ( data & (1<<0) )	// D4
		LPC_GPIO(LCD_D4PORT)->DATA |= (1<<LCD_D4);
	else
		LPC_GPIO(LCD_D4PORT)->DATA &= ~(1<<LCD_D4);

	if ( data & (1<<1) )	// D5
		LPC_GPIO(LCD_D5PORT)->DATA |= (1<<LCD_D5);
	else
		LPC_GPIO(LCD_D5PORT)->DATA &= ~(1<<LCD_D5);

	if ( data & (1<<2) )	// D6
		LPC_GPIO(LCD_D6PORT)->DATA |= (1<<LCD_D6);
	else
		LPC_GPIO(LCD_D6PORT)->DATA &= ~(1<<LCD_D6);

	if ( data & (1<<3) )	// D7
		LPC_GPIO(LCD_D7PORT)->DATA |= (1<<LCD_D7);
	else
		LPC_GPIO(LCD_D7PORT)->DATA &= ~(1<<LCD_D7);
}

// zapisuje bajt do HD44780
void lcd_write_byte(uint8_t data)
{
	// Ustawienie pinow portu LCD D4..D7 jako wyjscia
	data_dir_out();

#if USE_RW == 1
	CLR_RW;
#endif

	SET_E;
	lcd_sendHalf( data >> 4 );			// wyslanie starszej czesci bajtu danych D7..D4
	CLR_E;

	SET_E;
	lcd_sendHalf( data );				// wyslanie mlodszej czesci bajtu danych D3..D0
	CLR_E;

#if USE_RW == 1
	while( (check_BF() & (1<<7)) );
#else
	//_delay_us(120);
	delay_Us( 120 );
#endif

}

// wyslanie komendy do HD44780
void lcd_write_cmd(uint8_t command)
{
	CLR_RS;
	lcd_write_byte( command );
}

// wyslanie danych do HD44780
void lcd_write_data(uint8_t data)
{
	SET_RS;
	lcd_write_byte( data );
}




//**************************  FUNKCJE PUBLICZNE  ******************

#if USE_LCD_CHAR == 1
//----------------------------------------------------------------------------------------
//
//		 Wyslanie pojedynczego znaku do wyswietlacza LCD w postaci argumentu
//
//		 8 wlasnych znakow zdefiniowanych w CGRAM
//		 wysylamy za pomoca kodow 0x80 do 0x87 zamiast 0x00 do 0x07
//
//----------------------------------------------------------------------------------------
void lcd_char(char c)
{
	lcd_write_data( ( c>=0x80 && c<=0x87 ) ? (c & 0x07) : c);
}
#endif

//----------------------------------------------------------------------------------------
//
//		 Wyslanie stringa do wyswietlacza LCD
//
//		 8 wlasnych znakow zdefiniowanych w CGRAM
//		 wysylamy za pomoca kodow 0x80 do 0x87 zamiast 0x00 do 0x07
//
//----------------------------------------------------------------------------------------
void lcd_str(char *str)
{
	char znak;

	while ( (znak=*(str++)) )
		lcd_write_data( ( znak>=0x80 && znak<=0x87 ) ? (znak & 0x07) : znak);
}




#if USE_LCD_INT == 1
//----------------------------------------------------------------------------------------
//
//		 Wyswietla liczbe dziesietna na wyswietlaczu LCD
//
//----------------------------------------------------------------------------------------
void lcd_int(int val)
{
	char bufor[17];
	lcd_str( itoa(val, bufor, 10) );
}
#endif


#if USE_LCD_HEX == 1
//----------------------------------------------------------------------------------------
//
//		 Wyswietla liczbeâ€˘ szestnastkowa HEX na wyswietlaczu LCD
//
//----------------------------------------------------------------------------------------
void lcd_hex(int val)
{
	char bufor[17];
	lcd_str( itoa(val, bufor, 16) );
}
#endif


#if USE_LCD_DEFCHAR == 1
//----------------------------------------------------------------------------------------
//
//		Definicja wlasnego znaku na LCD z pamieci RAM
//
//		argumenty:
//		nr: 		- kod znaku w pamieci CGRAM od 0x80 do 0x87
//		*def_znak:	- wskaznik do tablicy 7 bajtow definiujacych znak
//
//----------------------------------------------------------------------------------------
void lcd_defchar(uint8_t nr, uint8_t *def_znak)
{
	register uint8_t i,c;
	lcd_write_cmd( 64+((nr&0x07)*8) );
	for(i=0;i<8;i++)
	{
		c = *(def_znak++);
		lcd_write_data(c);
	}
}
#endif


#if USE_LCD_LOCATE == 1
//----------------------------------------------------------------------------------------
//
//		Ustawienie kursora w pozycji Y-wiersz, X-kolumna
//
// 		Y = od 0 do 3
// 		X = od 0 do n
//
//		funkcja dostosowuje automatycznie adresy DDRAM
//		w zaleznosci od rodzaju wyswietlacza (ile posiada wierszy)
//
//----------------------------------------------------------------------------------------
void lcd_locate(uint8_t y, uint8_t x)
{
	switch(y)
	{
		case 0: y = LCD_LINE1; break;

#if (LCD_Y>1)
	    case 1: y = LCD_LINE2; break; // adres 1 znaku 2 wiersza
#endif
#if (LCD_Y>2)
    	case 2: y = LCD_LINE3; break; // adres 1 znaku 3 wiersza
#endif
#if (LCD_Y>3)
    	case 3: y = LCD_LINE4; break; // adres 1 znaku 4 wiersza
#endif
	}

	lcd_write_cmd( (0x80 + y + x) );
}
#endif


//----------------------------------------------------------------------------------------
//
//		Kasowanie ekranu wyswietlacza
//
//----------------------------------------------------------------------------------------
void lcd_cls(void)
{
	lcd_write_cmd( LCDC_CLS );

	#if USE_RW == 0
//		_delay_ms(4.9);
		delay_Ms( 5 );
	#endif
}


#if USE_LCD_CURSOR_HOME == 1
//----------------------------------------------------------------------------------------
//
//		Powrot kursora na poczatek
//
//----------------------------------------------------------------------------------------
void lcd_home(void)
{
	lcd_write_cmd( LCDC_CLS|LCDC_HOME );

	#if USE_RW == 0
//		_delay_ms(4.9);
		DelayMs( 5 );
	#endif
}
#endif

#if USE_LCD_CURSOR_ON == 1
//----------------------------------------------------------------------------------------
//
//		Wyswietlenie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_cursor_on(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON);
}

//----------------------------------------------------------------------------------------
//
//		Wylaczenie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_cursor_off(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif


#if USE_LCD_CURSOR_BLINK == 1
//----------------------------------------------------------------------------------------
//
//		WLacza miganie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_blink_on(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON|LCDC_BLINKON);
}

//----------------------------------------------------------------------------------------
//
//		WYlacza miganie kursora na LCD
//
//----------------------------------------------------------------------------------------
void lcd_blink_off(void)
{
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON);
}
#endif



//----------------------------------------------------------------------------------------
//
//		 ******* INICJALIZACJA WYSWIETLACZA LCD ********
//
//----------------------------------------------------------------------------------------
void lcd_init(void)
{
	ENABLE_TIMER;
	// inicjowanie pinow portow ustalonych do podlczenia z wyswietlaczem LCD
	// ustawienie wszystkich jako WYjscia
	data_dir_out();								// piny D4...D7 jako wyjscia
	LPC_GPIO(LCD_RSPORT)->DIR |= (1<<LCD_RS);	// pin RS wyjscie
	LPC_GPIO(LCD_EPORT)->DIR |= (1<<LCD_E);		// pin E wyjscie
	#if USE_RW == 1
		LPC_GPIO(LCD_RWPORT)->DIR |= (1<<LCD_RW);
	#endif

	// wyzerowanie wszystkich linii sterujacych
	LPC_GPIO(LCD_RSPORT)->DATA &= ~(1<<LCD_RS);
	LPC_GPIO(LCD_EPORT)->DATA &= ~(1<<LCD_E);
	#if USE_RW == 1
		LPC_GPIO(LCD_RWPORT)->DATA &= ~(1<<LCD_RW);
	#endif


	delay_Ms( 15 );
	LPC_GPIO(LCD_RSPORT)->DATA &= ~(1<<LCD_RS);
	LPC_GPIO(LCD_RWPORT)->DATA &= ~(1<<LCD_RW);

	// jeszcze nie mozna uzywac  Busy Flag
	lcd_sendHalf( LCDC_FUNC|LCDC_FUNC8B );
	//_delay_ms(4.1);
	delay_Ms( 5 );
	lcd_sendHalf( LCDC_FUNC|LCDC_FUNC8B );
	//_delay_us(100);
	delay_Us( 100 );
	lcd_sendHalf( LCDC_FUNC|LCDC_FUNC4B );
	//_delay_us(100);
	delay_Us( 100 );

	// juz mozna uzywac  Busy Flag
	// tryb 4-bitowy, 2 wiersze, znak 5x7
	lcd_write_cmd( LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7 );
	// wylaczenie kursora
	lcd_write_cmd( LCDC_ONOFF|LCDC_CURSOROFF );
	// wylaczenie wyswietlacza
	lcd_write_cmd( LCDC_ONOFF|LCDC_DISPLAYON );
	// przesuwanie kursora w prawo bez przesuwania zawartosci ekranu
	lcd_write_cmd( LCDC_ENTRY|LCDC_ENTRYR );

	// kasowanie ekranu
	lcd_cls();
}
