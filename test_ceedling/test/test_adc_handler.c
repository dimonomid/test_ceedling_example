

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

//-- unity: unit test framework
#include "unity.h"

//-- module being tested: dtp
#include "adc_handler.h"



/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/

static T_ADCHandler     _adc_handler;
static T_ADCHandler_Res _ctor_result;


/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/


/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
    T_ADCHandler_CtorParams params = {0};

    //-- 10-bit ADC
    params.max_counts = 0x3ff;

    //-- board-dependent maximum measured voltage: 10 Volts
    params.bsp_max_voltage = 10/*V*/ * ADC_HANDLER__SCALE_FACTOR__U;

    //-- the offset is 0 Volts
    params.nominal_add_volts = 0/*V*/;


    //-- construct the ADC handler, saving the result to _ctor_result
    _ctor_result = adc_handler__ctor(&_adc_handler, &params);
}

void tearDown(void)
{
    adc_handler__dtor(&_adc_handler);
}



/*******************************************************************************
 *    TESTS
 ******************************************************************************/

void test_ctor_ok(void)
{
    //-- check that constructor returned OK
    TEST_ASSERT_EQUAL_INT(ADC_HANDLER_RES__OK, _ctor_result);
}

void test_counts_to_voltage(void)
{
    T_ADCHandler_Voltage voltage;


    //------------------------------------------------------------------
    voltage = adc_handler__voltage__get_by_counts_value(
            &_adc_handler, 0
            );
    TEST_ASSERT_EQUAL_INT(0/*V*/ * ADC_HANDLER__SCALE_FACTOR__U, voltage);


    //------------------------------------------------------------------
    voltage = adc_handler__voltage__get_by_counts_value(
            &_adc_handler, 0x3ff
            );
    TEST_ASSERT_EQUAL_INT(10/*V*/ * ADC_HANDLER__SCALE_FACTOR__U, voltage);


    //------------------------------------------------------------------
    voltage = adc_handler__voltage__get_by_counts_value(
            &_adc_handler, 0x3ff / 3
            );
    TEST_ASSERT_EQUAL_INT((3.33/*V*/ * ADC_HANDLER__SCALE_FACTOR__U), voltage);
}



