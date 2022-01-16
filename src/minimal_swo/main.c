/**
 *  Simple cortex-debug enabled SWO example.
 *  Use the "WSL2-USB-SWO" vscode launch configuration to enable the cortex-debug
 *  SWO integrated terminal and graph plot.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "bsp_trace.h"


#define RED 8
#define GREEN 9

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000+4*n)))

// Default clock value (see p. 319 TRM)
#define HFRCO_IN_HZ 19000000
#define TIMER_1S 1*(HFRCO_IN_HZ)/38

/* Need to implement the  two Retarget IO functions with the read/write functions we want to use. */
int RETARGET_WriteChar(char c){
  return ITM_SendChar (c);
}

int RETARGET_ReadChar(void){
  return 0;
}

int main() 
{
    CHIP_Init();
    CMU_ClockEnable(cmuClock_GPIO, true);
    BSP_TraceSwoSetup();

    // Initialize LED driver
    GPIO_PinModeSet(gpioPortD, RED, gpioModePushPull, 0);
    GPIO_PinOutSet(gpioPortD, RED);

    ITM_SendChar('H');
    ITM_SendChar('E');
    ITM_SendChar('L');
    ITM_SendChar('L');
    ITM_SendChar('O');
    uint32_t i = 0;
    bool ascending = true;

    while (1)
    {
        // Ca. 1s blinking, depending on the oscillator setting
        for (int i = 0; i < TIMER_1S; i++)
        {
            __NOP();
        }

            
        while (ITM->PORT[1U].u32 == 0UL);
        
        if (i <= 10 && ascending)
        {
            ITM->PORT[1U].u32 = i++;
            if (i == 10)
            {
                ascending = false;
            }
        }
        else 
        {
            ITM->PORT[1U].u32 = i--;
            if ( i == 0)
            {
                ascending = true;
            }
        }

        GPIO_PinOutToggle(gpioPortD, RED);
    }
}
