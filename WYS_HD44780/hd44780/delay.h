/*
 * delay.h
 *
 *  Created on: 27 wrz 2015
 *      Author: badman
 */

#ifndef DELAY_H_
#define DELAY_H_

//#include "LPC11xx.h"
#define ENABLE_TIMER LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7)

void delay_Us(uint16_t delayIn_us);
void delay_Ms(uint16_t delayIn_ms);

#endif /* DELAY_H_ */
