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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    AVR/serial_lld.c
 * @brief   AVR low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   USARTC10 serial driver identifier.
 * @note    The name does not follow the convention used in the other ports
 *          (COMn) because a name conflict with the AVR headers.
 */
#if USE_XMEGA_USARTC1 || defined(__DOXYGEN__)
SerialDriver SDC1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
/************************************************************ FIX ME *****************************************************/
static const SerialConfig default_config = {
  0,
  0,
  0,
  0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(uint8_t sra, SerialDriver *sdp) {
  ioflags_t sts = 0;

  if (sra & USART_BUFOVF_bm)
    sts |= SD_OVERRUN_ERROR;
  if (sra & USART_PERR_bm)
    sts |= SD_PARITY_ERROR;
  if (sra & USART_FERR_bm)
    sts |= SD_FRAMING_ERROR;
  chSysLockFromIsr();
  chIOAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}



/************************************************************ FIX ME *****************************************************/
#if USE_XMEGA_USARTC1 || defined(__DOXYGEN__)
static void notifyC1(GenericQueue *qp) {

  (void)qp;
  USARTC1.CTRLA &= ~USART_DREINTLVL_gm;
  USARTC1.CTRLA |= USART_DREINTLVL_MED_gc;
}

/**
 * @brief   USART1 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
/************************************************************ FIX ME *****************************************************/
static void usartC1_init(const SerialConfig *config) {

  USARTC1.BAUDCTRLA = config->sc_baudctrla;
  USARTC1.BAUDCTRLB = config->sc_baudctrlb;
  USARTC1.CTRLC = config->sc_ctrlc;
  USARTC1.CTRLB = config->sc_ctrlb | USART_RXEN_bm | USART_TXEN_bm;
  USARTC1.CTRLA = USART_RXCINTLVL_MED_gc;
}

/**
 * @brief   USART1 de-initialization.
 */
static void usartC1_deinit(void) {

  USARTC1.CTRLA = 0;
  USARTC1.CTRLB &= ~(USART_TXEN_bm | USART_RXEN_bm);
  
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_XMEGA_USARTC1 || defined(__DOXYGEN__)
/**
 * @brief   USARTC1 RX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USARTC1_RXC_vect) {
	uint8_t status;
	CH_IRQ_PROLOGUE();
	
	status = USARTC1.STATUS;
	if ( status & (USART_FERR_bm | USART_PERR_bm | USART_BUFOVF_bm))
		set_error(status, &SDC1);
	chSysUnlockFromIsr();
	sdIncomingDataI(&SDC1, USARTC1.DATA);
	chSysUnlockFromIsr();
	
	CH_IRQ_PROLOGUE();
}

/**
 * @brief   USARTC1 TX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USARTC1_DRE_vect) {
	msg_t b;
	CH_IRQ_PROLOGUE();
	
	chSysLockFromIsr();
	b = sdRequestDataI(&SDC1);
	chSysUnlockFromIsr();
	if( b < Q_OK)
		USARTC1.CTRLA &= ~USART_DREINTLVL_gm;
	else
		USARTC1.DATA = b;
	
	CH_IRQ_PROLOGUE();
}
#endif /* USE_XMEGA_USARTC1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if USE_XMEGA_USARTC1
  sdObjectInit(&SDC1, NULL, notifyC1);
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

#if USE_XMEGA_USARTC1
  if (&SDC1 == sdp) {
    usartC1_init(config);
    return;
  }
#endif
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

#if USE_AVR_USARTC1
  if (&SDC1 == sdp)
    usartC1_deinit();
#endif
}

#endif /* HAL_USE_SERIAL */

/** @} */
