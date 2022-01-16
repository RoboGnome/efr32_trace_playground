/**
 *  Simple DWT/ITM based function profiling example with orbstat.
 *  Use the "WSL2-USB-Orbuculum" vscode launch configuration to 
 *  enable PC sampling using the DWT unit on the efr32.
 *  Set the PC sampling speed in the .vscode/launch.json to: dwtPostTap 1 & dwtPostReset 15
 *
 *  The source itm_trace_function.c contains the function instrumentation needed by orbstat to
 *  track the entry and exit of functions. 
 *   See http://shadetail.com/blog/swo-instrumentation-first-tunes/ for more information.
 *
 *  After starting the debugging session, let the cpu run and start orbstat with:
 *      orbstat -e build/bin/minimal_orbstat -v2 -z ./kcachegrind_test -s localhost:50001
 *
 *  You should see something like this: (dwtPostTap 0 & dwtPostReset 15)
 *       Server        : localhost:50001
 *       Delete Mat    : None
 *       Elf File      : build/bin/minimal_orbstat
 *       DOT file      : None
 *       ForceSync     : true
 *       Trace/File Ch : 30/29
 *       ITM In Sync (1)
 *       ITM In Sync (2)
 *       ITM In Sync (3)
 *       ITM In Sync (4)
 *       ITM In Sync (5)
 *       12872 records processed
 *       ITM In Sync (6)
 *       ITM In Sync (7)
 *       2944 records processed
 *
 *  After a couple of seconds stop orbstat and inspect the data with kcachegrind:
 *      OBJDUMP=/opt/toolchain/orbuculum/objdumpkc.sh kcachegrind kcachegrind_test
 * 
 *  See also: http://shadetail.com/blog/swo-instrumentation-building-the-orchestra/
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
