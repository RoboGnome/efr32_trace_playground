/**
 *  Simple DWT/ITM based PC sampling example with orbtop.
 *  Use the "WSL2-USB-Orbuculum" vscode launch configuration to 
 *  enable PC sampling using the DWT unit on the efr32.
 *  Set the PC sampling spped in the .vscode/launch.json to: dwtPostTap 0 & dwtPostReset 15
 *
 *  After starting the debugging session, let the cpu run and start orbtop with:
 *      orbtop -e build/bin/minimal_orbtop -v2 -c 15 -s localhost:50001
 *
 *  You should see something like this: (dwtPostTap 0 & dwtPostReset 15)
 *      51.38%  9573 addFloat
 *      48.33%  9005 addInt
 *      0.15%  29 GPIO_PinOutToggle
 *      0.12%  23 main
 *      -----------------
 *      99.98%     18630 of  18630  Samples
 *  
 *      [-S-H] Interval = 999mS
 *      Ovf=0  ITMSync= 40 TPIUSync=  0 ITMErrors=  0
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
    uint32_t t = TPI->ACPR;
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
