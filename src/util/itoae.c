/*******************************************************************************
 *    Description:   See header file
 *
 ******************************************************************************/


/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

#include "itoae.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>


/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/

/*******************************************************************************
 *    PUBLIC DATA
 ******************************************************************************/

/*******************************************************************************
 *    EXTERNAL DATA
 ******************************************************************************/

/*******************************************************************************
 *    EXTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * "Regular" unsigned-int-to-array implementation.
 * Copied from /opt/microchip/xc8/v1.35/sources/common/itoa.c
 */
static char *_utoa(char * buf, unsigned val, int base)
{
    unsigned v;
    char c;

    v = val;
    do {
        v /= base;
        buf++;
    } while(v != 0);

    *buf-- = 0;

    do {
        c = val % base;
        val /= base;

        if (c >= 10){
            c += 'A' - '0' - 10;
        }

        c += '0';

        *buf-- = c;
    } while (val != 0);

    return ++buf;
}

/**
 * Prepends existing string with leading chars `fill_char`, so that
 * resulting string has length `min_len`.
 *
 * @param p_buf
 *      Pointer to buffer with existing string
 * @param len
 *      Length of the string
 * @param min_len
 *      Minimal length of the string
 * @param fill_char
 *      Char to fill the leading space with
 *
 * @returns new string length
 */
static size_t _add_leading(
        uint8_t *p_buf, size_t len, size_t min_len, uint8_t fill_char
        )
{
    if (len < min_len){
        size_t diff = min_len - len;
        int i;

        //-- move existing data to the right
        for (i = len; i >= 0; i--){
            p_buf[i + diff] = p_buf[i];
        }

        //-- fill leading space with `fill_char`
        for (i = 0; i < diff; i++){
            p_buf[i] = fill_char;
        }

        //-- update length
        len = min_len;
    }

    return len;
}



/*******************************************************************************
 *    PUBLIC FUNCTIONS
 ******************************************************************************/

/*
 * See header for comments
 */
void itoae(uint8_t *p_buf, int value, int dpp, int min_len, uint8_t fill_char)
{
    //-- if the value is negative, put the minus sign at the beginning,
    //   and make the value positive
    if(value < 0) {
        *p_buf++ = '-';
        value = -value;
    }

    //-- perform "regular" itoa call
    _utoa(p_buf, value, 10);
    size_t len = strlen(p_buf);


    //-- if we need to add leading zeros for decimal point, do it
    len = _add_leading(p_buf, len, (dpp + 1), '0');

    //-- if we need to fill chars in the beginning, do it
    len = _add_leading(p_buf, len, min_len, fill_char);

    //-- if we need to put decimal point, do this
    if (dpp > 0){
        int i;
        for (i = 0; i < (dpp + 1/*null-terminate*/); i++){
            p_buf[len - i + 1] = p_buf[len - i];
        }
        p_buf[len - dpp] = '.';

        //-- since we've added a dot, length has just increased by 1
        len += 1;

        //-- put zeros around dot if necessary
        for (i = len - dpp - 1 - 1; i < len; i++){
            if (p_buf[i] == fill_char){
                p_buf[i] = '0';
            } else if (p_buf[i] != '.'){
                break;
            }
        }
    }
}



/*******************************************************************************
 *    end of file
 ******************************************************************************/



