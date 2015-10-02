/**
 * \file
 *
 * Module that converts ADC raw counts values to voltages and vice versa.
 * It also can auto-calibrate itself by 2 points, taking in account
 * calibration limits.
 */

#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

#include <stdint.h>



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/**
 * Scale factor used for voltage. Should correspond to
 * `#ADC_HANDLER__SCALE_DPP__U`
 */
#define     ADC_HANDLER__SCALE_FACTOR__U              100

/**
 * Decimal point position used for voltage. Should correspond to
 * `#ADC_HANDLER__SCALE_FACTOR__U`
 */
#define     ADC_HANDLER__SCALE_DPP__U                 2

/**
 * scale factor used for multiplier
 */
#define     ADC_HANDLER__SCALE_FACTOR__MUL            10


/**
 * The nominal multiplier, depending on maximum ADC value and
 * max board-dependent voltage.
 *
 * Multiplier shows "how many counts are contained in one volt".
 *
 * @param adc_max_value
 *      Maximum value that ADC can measure. For example, for 10-bit ADC
 *      it's `0x3ff`.
 *
 * @param bsp_max_voltage
 *      Maximum board-dependent voltage that could be measured.
 */
#define ADC_HANDLER__NOMINAL_MUL__GET(adc_max_value, bsp_max_voltage)   \
   (                                                                    \
    (                                                                   \
     (int32_t)(adc_max_value)                                           \
     * (int32_t)ADC_HANDLER__SCALE_FACTOR__MUL                          \
     * (int32_t)ADC_HANDLER__SCALE_FACTOR__U                            \
     )                                                                  \
    /                                                                   \
    (bsp_max_voltage)                                                   \
   )

/*******************************************************************************
 *    PUBLIC TYPES
 ******************************************************************************/

/**
 * Value in raw ADC counts, returned by the ADC hardware.
 */
typedef uint16_t   T_ADCHandler_CountsValue;

/**
 * Voltage value, multiplied by `ADC_HANDLER__SCALE_FACTOR__U`
 */
typedef int16_t    T_ADCHandler_Voltage;

/**
 * Calibration structure: contains the summand and the multiplier.
 */
typedef struct S_ADCHandler_Clb {
    /**
     * "how many counts are contained in one volt?", multiplied by
     * `#ADC_HANDLER__SCALE_FACTOR__MUL`
     */
    int16_t      mul;

    /**
     * Summand, in counts! (not in volts)
     * But it can be negative, so, don't use `#T_ADCHandler_CountsValue` here.
     */
    int16_t      add;
} T_ADCHandler_Clb;

/**
 * Structure representing minumum and maximum values
 */
struct S_ADCHandler_MinMax {
    int16_t min;
    int16_t max;
};

/**
 * Calibration limits, used for autocalibration.
 */
typedef struct S_ADCHandler_ClbLimits {

    /**
     * Multiplier limits, in percents of nominal mul, example:
     * {
     *     .min = -20, //%
     *     .max =  20, //%
     * }
     */
    struct S_ADCHandler_MinMax mul;

    /**
     * Summand limits, in volts multiplied by ADC_HANDLER__SCALE_FACTOR__U,
     * example:
     * {
     *     .min = (-0.5/V/ * ADC_HANDLER__SCALE_FACTOR__U),
     *     .max = ( 0.5/V/ * ADC_HANDLER__SCALE_FACTOR__U),
     * }
     */
    struct S_ADCHandler_MinMax add;

} T_ADCHandler_ClbLimits;

typedef struct S_ADCHandler T_ADCHandler;



/**
 * Result codes
 */
typedef enum E_ADCHandler_Res {
    ADC_HANDLER_RES__UNKNOWN,        //-- should never be returned

    ADC_HANDLER_RES__OK,

    ADC_HANDLER_RES__CLB_ERR__MUL_TOO_LOW,
    ADC_HANDLER_RES__CLB_ERR__MUL_TOO_HIGH,
    ADC_HANDLER_RES__CLB_ERR__ADD_TOO_LOW,
    ADC_HANDLER_RES__CLB_ERR__ADD_TOO_HIGH,

    ADC_HANDLER_RES__CLB_ERR__WRONG_PARAMS,

    ADC_HANDLER_RES__CLB_WRN__NO_LIMITS_GIVEN,
} T_ADCHandler_Res;


/**
 * Constructor params for ADC handler
 */
typedef struct S_ADCHandler_CtorParams {

    /**
     * Maximum value that ADC could return. Say, for 10-bit ADC it should be
     * 0x3ff.
     */
    T_ADCHandler_CountsValue   max_counts;

    /**
     * The board-dependent maximum voltage that could be measured, it
     * corresponds to the `max_counts`.
     *
     * It is only needed for calculation of nominal multiplier.
     */
    T_ADCHandler_Voltage      bsp_max_voltage;

    /**
     * Calibration data: summand and multiplier.
     *
     * Set just all zeros to use nominal. Nominal `mul` will be calculated by
     * `max_counts` and `bsp_max_voltage`. `add` will be copied from
     * nominal_add (see below)
     */
    T_ADCHandler_Clb  clb;

    /**
     * Nominal summand, in volts
     */
    T_ADCHandler_Voltage nominal_add_volts;

} T_ADCHandler_CtorParams;

/*******************************************************************************
 *   OBJECT CONTEXT
 ******************************************************************************/

/**
 * Object context
 */
struct S_ADCHandler {

    /**
     * See comment for `T_ADCHandler_CtorParams::max_counts`.
     */
    T_ADCHandler_CountsValue   max_counts;

    /**
     * See comment for `T_ADCHandler_CtorParams::bsp_max_voltage`.
     */
    T_ADCHandler_Voltage         bsp_max_voltage;

    /**
     * In counts! (not in volts)
     *
     * But it can be negative, so, don't use `T_ADCHandler_CountsValue` here.
     */
    int16_t      nominal_add;

    /**
     * Calibration data: summand and multiplier.
     */
    T_ADCHandler_Clb  clb;

};

/*******************************************************************************
 *   GLOBAL VARIABLES
 ******************************************************************************/

/*******************************************************************************
 *   CONSTRUCTOR, DESTRUCTOR PROTOTYPES
 ******************************************************************************/

/**
 * Constructor
 *
 * @return ADC_HANDLER_RES__OK if everything is ok, otherwise returns error code
 */
T_ADCHandler_Res adc_handler__ctor(
        T_ADCHandler *me, const T_ADCHandler_CtorParams *p_params
        );

/**
 * Desctructor
 */
void adc_handler__dtor(T_ADCHandler *me);

/*******************************************************************************
 *   PUBLIC METHODS PROTOTYPES
 ******************************************************************************/

/**
 * Returns nominal multiplier. See `#T_ADCHandler_Clb`
 */
int16_t adc_handler__nominal_mul__get(const T_ADCHandler *me);

/**
 * Returns value in counts, not in volts! But it can be negative, so,
 * we can't use T_ADCHandler_CountsValue here
 */
int16_t adc_handler__nominal_add__get(const T_ADCHandler *me);

/**
 * Convert ADC counts value to voltage value, using current calibration
 */
T_ADCHandler_Voltage adc_handler__voltage__get_by_counts_value(
        T_ADCHandler *me, T_ADCHandler_CountsValue counts_value
        );

/**
 * Convert voltage value to ADC counts value, using current calibration
 */
T_ADCHandler_CountsValue adc_handler__counts_value__get_by_voltage(
        T_ADCHandler *me, T_ADCHandler_Voltage voltage
        );

/**
 * Get pointer to current calibration structure
 */
const T_ADCHandler_Clb *adc_handler__clb__get_current(const T_ADCHandler *me);


/**
 * Get nominal calibration, based on `bsp_max_voltage` and `nominal_add`.
 */
T_ADCHandler_Clb adc_handler__clb__get_nominal(const T_ADCHandler *me);

/**
 * Set calibration
 */
void adc_handler__clb__set(T_ADCHandler *me, const T_ADCHandler_Clb *p_clb);

/**
 * Set calibration multiplier:
 * "how many counts are contained in one volt?", multiplied by
 * `ADC_HANDLER__SCALE_FACTOR__MUL`
 */
void adc_handler__clb_mul__set(T_ADCHandler *me, int16_t mul);

/**
 * Set calibration summand, in ADC counts (not in volts)
 * But it can be negative, so, don't use `T_ADCHandler_CountsValue` here.
 */
void adc_handler__clb_add__set(T_ADCHandler *me, int16_t add);


#endif // _ADC_HANDLER_H

/*******************************************************************************
 *   end of file
 ******************************************************************************/


