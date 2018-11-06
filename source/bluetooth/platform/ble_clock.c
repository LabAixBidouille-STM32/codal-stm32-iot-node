
#include "stm32.h"
#include "codal_target_hal.h"
#include "ble_clock.h"

const uint32_t CLOCK_SECOND = 1000;

/*---------------------------------------------------------------------------*/

void Clock_Init(void)
{
  //Not Used
}

/*---------------------------------------------------------------------------*/

tClockTime Clock_Time(void)
{
	return HAL_GetTick();
}

/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 1 ms.
 *
 */
void Clock_Wait(uint32_t i)
{
	target_wait(i);
}
/*---------------------------------------------------------------------------*/
