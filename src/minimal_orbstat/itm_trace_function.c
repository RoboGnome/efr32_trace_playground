#include "em_device.h"  
#define TRACE_CHANNEL (30)
#define DELAY_TIME (0)

__attribute__ ((no_instrument_function))
            inline  void __cyg_profile_func_enter (void *this_fn, void *call_site)
{
    if (!(ITM->TER&(1<<TRACE_CHANNEL))) return;
    uint32_t oldIntStat=__get_PRIMASK();

    // This is not atomic, but by using the stack for
    //storing oldIntStat it doesn't matter
    __disable_irq();
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);

    // This is CYCCNT - number of cycles of the CPU clock  
    ITM->PORT[TRACE_CHANNEL].u32 = ((*((uint32_t *)0xE0001004))&0x03FFFFFF)|0x40000000;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)(call_site)&0xFFFFFFFE;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);

    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)this_fn&0xFFFFFFFE;
    for (uint32_t d=0; d<DELAY_TIME; d++) asm volatile ("NOP");

    __set_PRIMASK(oldIntStat);
}

__attribute__ ((no_instrument_function))
            inline  void __cyg_profile_func_exit (void *this_fn, void *call_site)
{
    if (!(ITM->TER&(1<<TRACE_CHANNEL))) return;
    uint32_t oldIntStat=__get_PRIMASK();
    __disable_irq();
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = ((*((uint32_t *)0xE0001004))&0x03FFFFFF)|0x50000000;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)(call_site)&0xFFFFFFFE;
    while (ITM->PORT[TRACE_CHANNEL].u32 == 0);
    ITM->PORT[TRACE_CHANNEL].u32 = (uint32_t)this_fn&0xFFFFFFFE;
    for (uint32_t d=0; d < DELAY_TIME; d++) asm volatile ("NOP");
    __set_PRIMASK(oldIntStat);
}