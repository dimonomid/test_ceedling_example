

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

//-- unity: unit test framework
#include "unity.h"

//-- module being tested
#include "appl_adc.h"

//-- other modules that need to be compiled
#include "adc_handler.h"

//-- mocked modules
#include "mock_bsp_adc.h"


/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/


/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/


/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
    //-- before each test, re-initialize appl_adc module
    appl_adc__init();
}

void tearDown(void)
{
    //-- nothing to do here
}



/*******************************************************************************
 *    TESTS
 ******************************************************************************/

void test_voltage_get(void)
{
    //-- We expect bsp_adc__value__get() to be called:
    bsp_adc__value__get_ExpectAndReturn(
            //-- the argument that is expected to be given to
            //   bsp_adc__value__get()
            APPL_ADC_CH__I_SETT,

            //-- and the value that bsp_adc__value__get() should return
            (0x3ff / 2)
            );

    //-- actually call the function being tested, that should perform
    //   all pending expected calls
    T_ADCHandler_Voltage voltage = appl_adc__voltage__get(
            APPL_ADC_CH__I_SETT
            );

    //-- check the voltage returned (we assume that adc_handler is initialized
    //   with the same params, where 0x3ff is the maximum ADC value, and
    //   it corresponds to the value (10 * ADC_HANDLER__SCALE_FACTOR__U))
    TEST_ASSERT_EQUAL_INT((5 * ADC_HANDLER__SCALE_FACTOR__U), voltage);
}



