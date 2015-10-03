

/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/

//-- unity: unit test framework
#include "unity.h"

//-- module being tested
#include "appl_adc.h"

//-- mocked modules
#include "mock_bsp_adc.h"
#include "mock_adc_handler.h"


/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/

/*******************************************************************************
 *    EXTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/

extern T_ADCHandler *_appl_adc__adc_handler__get(enum E_ApplAdcChannel channel);




/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/


/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/

static T_ADCHandler_Voltage _get_by_counts_value_Callback(
        T_ADCHandler *me,
        T_ADCHandler_CountsValue counts_value,
        int num_calls
        )
{
    T_ADCHandler_Voltage ret = 0;

    switch (num_calls){
        case 0:
            if (counts_value != 123){

                //-- We can check whatever we want here. For example, we may
                //   check the data pointed to by "me", but NOTE that currently
                //   it is just zeros, since we have mocked adc_handler__ctor()
                //   as well, so the original constructor isn't called, and
                //   instances are left unitialized.

                TEST_FAIL_MESSAGE(
                        "adc_handler__voltage__get_by_counts_value() was called "
                        "with wrong counts_value"
                        );
            }

            ret = 456;
            break;

        default:
            TEST_FAIL_MESSAGE(
                    "adc_handler__voltage__get_by_counts_value() was called "
                    "too many times"
                    );
            break;
    }

    return ret;
}




/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

void setUp(void)
{
    //-- ADC handler constructor is going to be called for each channel.
    //   Mocked constructors all return ADC_HANDLER_RES__OK.
    enum E_ApplAdcChannel channel;
    for (channel = 0; channel < APPL_ADC_CH_CNT; channel++){
        adc_handler__ctor_IgnoreAndReturn(ADC_HANDLER_RES__OK);
    }

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
            123
            );

    //-- Expect call to adc_handler__voltage__get_by_counts_value()
    adc_handler__voltage__get_by_counts_value_StubWithCallback(
            _get_by_counts_value_Callback
            );

    //-- actually call the function being tested, that should perform
    //   all pending expected calls
    T_ADCHandler_Voltage voltage = appl_adc__voltage__get(
            APPL_ADC_CH__I_SETT
            );

    //-- check the voltage returned (it should be 456 from the mock above)
    TEST_ASSERT_EQUAL_INT(456, voltage);
}



