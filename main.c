/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: CCU8 Timer Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2022, Infineon Technologies AG
* All rights reserved.                        
*                                             
* Boost Software License - Version 1.0 - August 17th, 2003
* 
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
* 
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*                                                                              
*****************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include <stdio.h>
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/* Define macros for  XMC13x and XMC14x Boot kit */
#if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     3
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN
#endif

/* Define macros for XMC48x, XMC47x, XMX43x, XMC45x Relax kits, XMC44x and XMC42x PLT2GO kits */
#if (UC_FAMILY == XMC4)
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR1_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR1_IRQN
#endif

/*******************************************************************************
* Global Variables
*******************************************************************************/
static volatile bool timer_interrupt_flag = false;

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_XMC_DEBUG_PRINT (0)

/* Define macro to set the loop count before printing debug messages */
#if ENABLE_XMC_DEBUG_PRINT
#define DEBUG_LOOP_COUNT_MAX                    2
static bool ENTER_LOOP = false;
#endif

/*******************************************************************************
* Function Name: TIMER_0_PERIOD_MATCH_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CCU8 timer period match interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void TIMER_0_PERIOD_MATCH_EVENT_HANDLER(void)
{
    timer_interrupt_flag = true;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.It sets up a CCU8 timer to period match interrupt.
* The main while loop checks for the elapsed time due to CCU8 timer period match
* event and toggles an LED at approximately 1Hz.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize printf retarget */
    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    #if ENABLE_XMC_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    #if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    /* Set priority */
    NVIC_SetPriority(TIMER_0_PERIOD_MATCH_EVENT_IRQN, TIMER_0_PERIOD_MATCH_EVENT_PRIORITY);
    #endif

    #if (UC_SERIES == XMC48) || (UC_SERIES == XMC47) || (UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC42)
    /* Set priority */
    NVIC_SetPriority(TIMER_0_PERIOD_MATCH_EVENT_IRQN, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TIMER_0_PERIOD_MATCH_EVENT_PRIORITY, 0));
    #endif

    /* Enable IRQ */
    NVIC_EnableIRQ(TIMER_0_PERIOD_MATCH_EVENT_IRQN);

    /* Infinite loop */
    while(1)
    {
        /* Check if timer elapsed (interrupt fired) and toggle the LED */
        if (timer_interrupt_flag)
        {
            /* Clear the flag */
            timer_interrupt_flag = false;
            
            /* Toggle the USER LED state */
            XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

            #if ( ENABLE_XMC_DEBUG_PRINT )
            if(!ENTER_LOOP)
            {
                printf("User LED toggled\r\n");
                ENTER_LOOP = true;
            }
            #endif
        }
    }
}

/* [] END OF FILE */
