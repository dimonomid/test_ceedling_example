/**
 * \file
 *
 * MCU-specific ADC routines: get results from the ADC peripheral and expose
 * results to clients as raw ADC counts.
 */

#ifndef _BSP_ADC_H
#define _BSP_ADC_H

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

#include <stdint.h>

#include "appl_adc_channels.h"



/*******************************************************************************
 *    PUBLIC TYPES
 ******************************************************************************/

/**
 * Type that is used for ADC raw counts.
 */
typedef uint16_t T_BspAdcCounts;



/*******************************************************************************
 *    GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

/**
 * Perform module initialization, including the hardware initialization
 */
void bsp_adc__init(void);

/**
 * Returns raw ADC counts for the specified channel
 */
T_BspAdcCounts bsp_adc__value__get(enum E_ApplAdcChannel channel_num);


/**
 * To be repeatedly called from the application's super loop: stores current
 * measurement when it's ready and switches between different channels as
 * appropriate
 */
void bsp_adc__proceed(void);

#endif // _BSP_ADC_H


/*******************************************************************************
 *    end of file
 ******************************************************************************/


