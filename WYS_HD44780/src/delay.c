/*
 * delay.c
 *
 *  Created on: 27 wrz 2015
 *      Author: badman
 */

#include "LPC11xx.h"

// 1 MHz preskaler
#define MHZ_PRESCALE    (SystemCoreClock / LPC_SYSCON->SYSAHBCLKDIV / 1000000)

// 1 kHz preskaler
#define KHZ_PRESCALE    (SystemCoreClock / LPC_SYSCON->SYSAHBCLKDIV / 1000)

void delay_Us(uint16_t delayIn_us)
{
	LPC_TMR16B0->TCR = 0x02;				// wyzerowanie TC przy nastepnym cyklu  (PCLK)
	LPC_TMR16B0->PR  = MHZ_PRESCALE - 1;	// preskaler na 1 us
	LPC_TMR16B0->MR0 = delayIn_us;			// odlicz ilosc us
	LPC_TMR16B0->MCR = 0x04;				// zatrzymaj zegar kiedy TC = MR0 i wyzeruj bit 0 w rejestrze TCR
	LPC_TMR16B0->TCR = 0x01;				// start zegara

	while (LPC_TMR16B0->TCR & 0x01);		// czekaj dopoki zegar zostanie zatrzymany
}

void delay_Ms(uint16_t delayIn_ms)
{
	LPC_TMR16B0->TCR = 0x02;				// wyzerowanie TC przy nastepnym cyklu  (PCLK)
	LPC_TMR16B0->PR  = KHZ_PRESCALE - 1;	// preskaler na 1 ms
	LPC_TMR16B0->MR0 = delayIn_ms;			// odlicz ilosc us
	LPC_TMR16B0->MCR = 0x04;				// zatrzymaj zegar kiedy TC = MR0 i wyzeruj bit 0 w rejestrze TCR
	LPC_TMR16B0->TCR = 0x01;				// start zegara

	while (LPC_TMR16B0->TCR & 0x01);		// czekaj dopoki zegar zostanie zatrzymany
}
