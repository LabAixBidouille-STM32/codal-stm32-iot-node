#ifndef __STM32L4xx_IT_BLE_H
#define __STM32L4xx_IT_BLE_H


#ifdef __cplusplus
extern "C"{
#endif

#include "stm32l4xx_hal.h"
#include "stm32.h"

void RTC_WKUP_IRQHandler(void); 
void EXTI9_5_IRQHandler(void);         /* BNRG_SPI_EXTI_IRQHandler   */
void DMA2_Channel1_IRQHandler(void);   /* BNRG_SPI_RX_DMA_IRQHandler */
void DMA2_Channel2_IRQHandler(void);   /* BNRG_SPI_TX_DMA_IRQHandler */

#ifdef __cplusplus
}
#endif

#endif