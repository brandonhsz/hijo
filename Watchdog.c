/*
 * Watchdog.c
 *
 *  Created on: Jun 5, 2018
 *      Author: Usuario
 */

#include "Watchdog.h"
#include "fsl_cop.h"
#include "fsl_rcm.h"
#include "init_config_functions.h"

//this is used to enable console message printing
#define CONSOLE_DOG_DEBUGGING 0

/////////////////////////function prototypes////////////////////////////////////
void WD_forceReset(SIM_Type *base);
/////////////////////end of functions prototypes////////////////////////////////

///////////////////////global variables definition//////////////////////////////
cop_config_t g_configCop;  //
///////////////////end of global variables definition///////////////////////////

////////////////////////public functions definitions////////////////////////////
void Watchdog_initModuleConfiguration()
{
    COP_GetDefaultConfig(&g_configCop); //default Watchdog configuration
}

//TODO: Set option to enable WD in STOP mode
void Watchdog_setDifferentTimer(DogTime_t TimeSelection)
{
    /*This function modifies the Watchdog timeout mode and cycles according to
     the 12-1 table (COP configuration options), from Chapter 12 of the
     KL27 Sub-Family Reference Manual*/
    switch (TimeSelection)
    {
        case DogTime_32ms:
            COP_GetDefaultConfig(&g_configCop); //32ms Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
            g_configCop.timeoutMode = kCOP_ShortTimeoutMode;
        break;

        case DogTime_256ms:
            COP_GetDefaultConfig(&g_configCop); //256ms Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power8CyclesOr2Power16Cycles;
            g_configCop.timeoutMode = kCOP_ShortTimeoutMode;
        break;

        case DogTime_1024ms:
            COP_GetDefaultConfig(&g_configCop); //1.024s Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power10CyclesOr2Power18Cycles;
            g_configCop.timeoutMode = kCOP_ShortTimeoutMode;
        break;

        case DogTime_8192ms:
            COP_GetDefaultConfig(&g_configCop); //8.192s Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power5CyclesOr2Power13Cycles;
            g_configCop.timeoutMode = kCOP_LongTimeoutMode;
        break;

        case DogTime_65536ms:
            COP_GetDefaultConfig(&g_configCop); //1.09min Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power8CyclesOr2Power16Cycles;
            g_configCop.timeoutMode = kCOP_LongTimeoutMode;
        break;

        case DogTime_262144ms:
            COP_GetDefaultConfig(&g_configCop); //4.36min Watchdog configuration
            g_configCop.timeoutCycles = kCOP_2Power10CyclesOr2Power18Cycles;
            g_configCop.timeoutMode = kCOP_LongTimeoutMode;
        break;
    }

   /** TODO: This configuration was hardcoded for specific operation of
    * the WD in the mock-up, please make a function to enable this.*/
    g_configCop.enableStop = true;

}

void Watchdog_dogAlert()
{
    COP_Init(SIM, &g_configCop);  //Watchdog module enabling
    #if CONSOLE_DOG_DEBUGGING
        PRINTF("\r\nThe dog is now guarding the system!\r\n");
    #endif
}

void Watchdog_SleepTheDog()
{
    COP_Disable(SIM);  //Watchdog module disabling
    #if CONSOLE_DOG_DEBUGGING
        PRINTF("\r\nThe dog is now sleeping, so it won't protect the system!\r\n");
    #endif
}

void Watchdog_feedTheDog(DogFood_t typeOfFood)
{
    if (normalFood == typeOfFood)
    {
        COP_Refresh(SIM);  //Watchdog normal refreshing
        #if CONSOLE_DOG_DEBUGGING
            PRINTF("\r\nThe dog has been fed!\r\n");
        #endif

    } else if (resetFood == typeOfFood)
    {
        WD_forceReset(SIM);  //Watchdog wrong refreshing = system reboot
        #if CONSOLE_DOG_DEBUGGING
            PRINTF("\r\nThe dog will reset the module\r\n");
        #endif
    }
}

void Watchdog_checkIfReset(uint8_t * Watchdog_test, void * function)
{
    /* Check if WDOG reset occurred */
    if (RCM_GetPreviousResetSources(RCM) & kRCM_SourceWdog)
    {
        Watchdog_SleepTheDog(); //sends to sleep the dog
        *Watchdog_test = true;	//sets the passed-by-reference variable to true

        #if CONSOLE_DOG_DEBUGGING
        PRINTF("\nReset due to Watchdog timeout\r\n");
#endif

        //function pointer to the passed by reference function
        if(function != NULL){
           void (*genericFunctionPointer)(void) = function;
           //the function reference input argument is called
           genericFunctionPointer();
        }
    }
}
////////////////////end of public functions definitions/////////////////////////

/////////////////////////mechanisms definition//////////////////////////////////
void WD_forceReset(SIM_Type *base)
{
    uint32_t primaskValue = 0U;

    /* Disable the global interrupt to protect refresh sequence */
    primaskValue = DisableGlobalIRQ();

    /*Incorrect feeding equals system reboot, as expected*/
    base->SRVCOP = 0xFF;

    /* Back to normal global interrupt enabling */
    EnableGlobalIRQ(primaskValue);
}
////////////////////end of mechanisms definition////////////////////////////////
