/**
 *  Simple blinky example using the sdk_support library on a thunderboard sense 2
 * 
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
#include "add.h"

#define RED 8
#define GREEN 9

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000+4*n)))

// Default clock value (see p. 319 TRM)
#define HFRCO_IN_HZ 19000000
#define TIMER_1S 1*(HFRCO_IN_HZ)/38

int main() 
{
    CHIP_Init();
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Initialize LED driver
    GPIO_PinModeSet(gpioPortD, RED, gpioModePushPull, 0);
    GPIO_PinOutSet(gpioPortD, RED);

    uint32_t i = 0;
    bool ascending = true;

    while (1)
    {
        addInt(1, 2);
        addFloat(1.0f, 2.0f);

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
