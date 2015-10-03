

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
#include "mock_xc.h"


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

    //-- Expect call to adc_handler__voltage__get_by_counts_value(),
    //   ignoring arguments. The mocked version just returns 456.
    adc_handler__voltage__get_by_counts_value_ExpectAndReturn(
            //-- pointer to the appropriate ADC handler instance
            _appl_adc__adc_handler__get(APPL_ADC_CH__I_SETT),
            //-- value returned from bsp_adc__value__get()
            123,
            //-- returned value in Volts
            456
            );

    //-- actually call the function being tested, that should perform
    //   all pending expected calls
    T_ADCHandler_Voltage voltage = appl_adc__voltage__get(
            APPL_ADC_CH__I_SETT
            );

    //-- check the voltage returned (it should be 456 from the mock above)
    TEST_ASSERT_EQUAL_INT(456, voltage);
}

void test_voltage_get_wrong_channel_number(void)
{
    //-- we expect __builtin_software_breakpoint() to be called ...
    __builtin_software_breakpoint_Expect();

    //-- ... when we call appl_adc__voltage__get() with illegal
    //   channel number.
    appl_adc__voltage__get(
            APPL_ADC_CH_CNT
            );
}



