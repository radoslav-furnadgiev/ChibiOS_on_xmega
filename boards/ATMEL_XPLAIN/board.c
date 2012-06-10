/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
{
#if defined(PORTA)
  {VAL_PORTA, VAL_DDRA},
#endif
#if defined(PORTB)
  {VAL_PORTB, VAL_DDRB},
#endif
#if defined(PORTC)
  {VAL_PORTC, VAL_DDRC},
#endif
#if defined(PORTD)
  {VAL_PORTD, VAL_DDRD},
#endif
#if defined(PORTE)
  {VAL_PORTE, VAL_DDRE},
#endif
#if defined(PORTF)
  {VAL_PORTF, VAL_DDRF},
#endif
#if defined(PORTG)
  {VAL_PORTG, VAL_DDRG},
#endif
};
#endif /* HAL_USE_PAL */

/**
 * @brief   Timer0 interrupt handler.
 */
CH_IRQ_HANDLER(TCC0_OVF_vect) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/**
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * External interrupts setup, all disabled initially.
   */
//  EICRA  = 0x00;
//  EICRB  = 0x00;
//  EIMSK  = 0x00;

  /*
   * Enables Idle mode for SLEEP instruction.
   */
//  MCUCR  = (1 << SE);

	TCC0.CTRLA = TC_CLKSEL_DIV8_gc;
	TCC0.PER = F_CPU / 8 / CH_FREQUENCY - 1;
	TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc;
	PMIC.CTRL |= PMIC_HILVLEN_bm;
//  This (enabling interrupts) happens elsewhere 
//	chSysUnlock();
}
