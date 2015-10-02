/**
 * \file
 *
 * Extended version of itoa, see comments for itoae() below
 */

#ifndef _MY_ITOA_H
#define _MY_ITOA_H

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

#include <stdint.h>


/*******************************************************************************
 *    PUBLIC TYPES
 ******************************************************************************/

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
 * Itoa a bit extended: allows to set fixed width of the string
 * (effectively allowing us to align text by right edge),
 * and allows to put decimal point at some fixed position.
 *
 * @param p_buf
 *      where to save string data
 * @param value
 *      value to convert to string
 * @param dpp
 *      decimal point position. If 0, then no decimal point is put.
 *      if 1, then it is put one digit from the right side, etc.
 * @param width
 *      minimum width of the string. If actual string is shorter than
 *      specified width, then leftmost characters are filled with
 *      `fill_char`.
 * @param fill_char
 *      character to fill "extra" space
 */
void itoae(uint8_t *p_buf, int value, int dpp, int width, uint8_t fill_char);


#endif // _MY_ITOA_H


/*******************************************************************************
 *    end of file
 ******************************************************************************/


