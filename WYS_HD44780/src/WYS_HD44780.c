/*
===============================================================================
 Name        : WYS_HD44780.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include "hd44780.h"


int main(void) {

	lcd_init();
	lcd_str("Hello");

    while(1) {

    }
    return 0 ;
}
