/*******************************************************************************
 *    Description:   See header file
 *
 ******************************************************************************/


/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

#include "adc_handler.h"
#include <string.h>
#include "util_macros.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE METHOD PROTOTYPES
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
 *    EXTERNAL METHOD PROTOTYPES
 ******************************************************************************/

/*******************************************************************************
 *    PROTECTED METHODS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE METHODS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE STATIC METHODS
 ******************************************************************************/

static T_ADCHandler_CountsValue _counts_value__get_by_voltage(
        T_ADCHandler_Voltage voltage,
        int16_t mul,
        int16_t add
        )
{

    int32_t counts_scaled = voltage * mul;
    T_ADCHandler_CountsValue counts_value =
        DFC_INT_DIV(
                DFC_INT_DIV(
                    counts_scaled,
                    ADC_HANDLER__SCALE_FACTOR__U
                    ),
                ADC_HANDLER__SCALE_FACTOR__MUL
                );

    counts_value += add;

    return counts_value;
}

static T_ADCHandler_Voltage _voltage__get_by_counts_value(
        T_ADCHandler_CountsValue counts_value, int16_t mul, int16_t add
        )
{
    int32_t voltage = counts_value - add;
    voltage =
        (
         (
          voltage
          * ADC_HANDLER__SCALE_FACTOR__U
          * ADC_HANDLER__SCALE_FACTOR__MUL/*compensate factor in mul*/
         )
         + (mul >> 1)
        )
        /
        mul;

    return (T_ADCHandler_Voltage)voltage;
}



/*******************************************************************************
 *    CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

/**
 * Constructor
 *
 */
T_ADCHandler_Res adc_handler__ctor(
        T_ADCHandler *me, const T_ADCHandler_CtorParams *p_params
        )
{
    T_ADCHandler_Res ret = ADC_HANDLER_RES__OK;
    memset(me, 0x00, sizeof(T_ADCHandler));

    //-- some construct code

    me->max_counts = p_params->max_counts;
    me->bsp_max_voltage = p_params->bsp_max_voltage;

    if (p_params->nominal_add_volts == 0){
        me->nominal_add   = 0;
    } else {
        int16_t tmp_mul = ADC_HANDLER__NOMINAL_MUL__GET(
                p_params->max_counts,
                p_params->bsp_max_voltage
                );

        me->nominal_add   =
            /* !!! NOTE the minus sign! */
            -_counts_value__get_by_voltage(
                    p_params->nominal_add_volts,
                    tmp_mul,
                    0
                    );
    }

    me->clb           = p_params->clb;

    //-- if clb data is zero, then set nominal values
    //   (maybe check only mul here? since, add is typically zero)
    if (me->clb.add == 0 && me->clb.mul == 0){
        T_ADCHandler_Clb nominal_clb = adc_handler__clb__get_nominal(me);
        adc_handler__clb__set(
                me,
                &nominal_clb
                );
    }

    return ret;
}


/**
 * Desctructor
 */
void adc_handler__dtor(T_ADCHandler *me)
{
}

/*******************************************************************************
 *    PUBLIC METHODS
 ******************************************************************************/

/*
 * See comments in the header file
 */
T_ADCHandler_Voltage adc_handler__voltage__get_by_counts_value(
        T_ADCHandler *me, T_ADCHandler_CountsValue counts_value
        )
{
    return _voltage__get_by_counts_value(
            counts_value,
            me->clb.mul,
            me->clb.add
            );
}

/*
 * See comments in the header file
 */
T_ADCHandler_CountsValue adc_handler__counts_value__get_by_voltage(
        T_ADCHandler *me, T_ADCHandler_Voltage voltage
        )
{
    return _counts_value__get_by_voltage(
            voltage,
            me->clb.mul,
            me->clb.add
            );
}

/*
 * See comments in the header file
 */
const T_ADCHandler_Clb *adc_handler__clb__get_current(const T_ADCHandler *me)
{
    return &me->clb;
}

int16_t adc_handler__nominal_mul__get(const T_ADCHandler *me)
{
    return ADC_HANDLER__NOMINAL_MUL__GET(
            me->max_counts,
            me->bsp_max_voltage
            );
}

/*
 * See comments in the header file
 */
int16_t adc_handler__nominal_add__get(const T_ADCHandler *me)
{
    return me->nominal_add;
}



/*
 * See comments in the header file
 */
T_ADCHandler_Clb adc_handler__clb__get_nominal(const T_ADCHandler *me)
{
    T_ADCHandler_Clb ret;
    ret.add = adc_handler__nominal_add__get(me);
    ret.mul = adc_handler__nominal_mul__get(me);

    return ret;
}

/*
 * See comments in the header file
 */
void adc_handler__clb__set(T_ADCHandler *me, const T_ADCHandler_Clb *p_clb)
{
    me->clb = *p_clb;
}

/*
 * See comments in the header file
 */
void adc_handler__clb_mul__set(T_ADCHandler *me, int16_t mul)
{
    me->clb.mul = mul;
}

/*
 * See comments in the header file
 */
void adc_handler__clb_add__set(T_ADCHandler *me, int16_t add)
{
    me->clb.add = add;
}



/*******************************************************************************
 *  end of file
 ******************************************************************************/


