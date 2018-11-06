/**
  ******************************************************************************
  * @file    bluenrg_targets.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    24-July-2015
  * @brief   This header file is intended to manage the differences between 
  *          the different supported base-boards which might mount the
  *          X_NUCLEO_IDB0XA1 BlueNRG BLE Expansion Board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
 
/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef _BLUENRG_TARGETS_H_
#define _BLUENRG_TARGETS_H_

#if !defined(BLUENRG_PIN_SPI_MOSI)
#define BLUENRG_PIN_SPI_MOSI   (codal::PinNumber::PC_12)
#endif
#if !defined(BLUENRG_PIN_SPI_MISO)
#define BLUENRG_PIN_SPI_MISO   (codal::PinNumber::PC_11)
#endif
#if !defined(BLUENRG_PIN_SPI_nCS)
#define BLUENRG_PIN_SPI_nCS    (codal::PinNumber::PD_13)
#endif
#if !defined(BLUENRG_PIN_SPI_RESET)
#define BLUENRG_PIN_SPI_RESET  (codal::PinNumber::PA_8)
#endif
#if !defined(BLUENRG_PIN_SPI_IRQ)
#define BLUENRG_PIN_SPI_IRQ    (codal::PinNumber::PE_6)
#endif

/* NOTE: Refer to README for further details regarding BLUENRG_PIN_SPI_SCK */
#if !defined(BLUENRG_PIN_SPI_SCK)
#define BLUENRG_PIN_SPI_SCK    (codal::PinNumber::PC_10)
#endif

/* NOTE: Stack Mode 0x04 allows Simultaneous Scanning and Advertisement (SSAdv)
 *       Mode 0x01: slave or master, 1 connection
 *       Mode 0x02: slave or master, 1 connection
 *       Mode 0x03: master/slave, 8 connections
 *       Mode 0x04: master/slave, 4 connections (simultaneous scanning and advertising)
 *       Check Table 285 of 
 *       BlueNRG-MS Bluetooth LE stack application command interface (ACI) User Manual (UM1865) at st.com
 */
#if !defined(BLUENRG_STACK_MODE)
#define BLUENRG_STACK_MODE (0x04)
#endif


#define BLUENRG_READY_STATE 0x02

#define HEADER_SIZE 5

#define CS_PULSE_LENGTH (CS_PULSE_625NS_NBR_CYCLES_REQ/4)
#define DUMMY_RAM_ADDRESS_TO_READ (0x20000000)

  /* SPI Private defines-----------------------------------------------------------*/
  // SPI Instance
#define BNRG_SPI_INSTANCE               SPI3
#define BNRG_SPI_CLK_ENABLE()           __HAL_RCC_SPI3_CLK_ENABLE()

  // SPI Configuration
#define BNRG_SPI_MODE                   SPI_MODE_MASTER
#define BNRG_SPI_DIRECTION              SPI_DIRECTION_2LINES
#define BNRG_SPI_DATASIZE               SPI_DATASIZE_8BIT
#define BNRG_SPI_CLKPOLARITY            SPI_POLARITY_LOW
#define BNRG_SPI_CLKPHASE               SPI_PHASE_1EDGE
#define BNRG_SPI_NSS                    SPI_NSS_SOFT
#define BNRG_SPI_FIRSTBIT               SPI_FIRSTBIT_MSB
#define BNRG_SPI_TIMODE                 SPI_TIMODE_DISABLE
#define BNRG_SPI_CRCPOLYNOMIAL          7
#define BNRG_SPI_BAUDRATEPRESCALER      SPI_BAUDRATEPRESCALER_2 /**< Keep clock running at 8Mhz */
#define BNRG_SPI_CRCCALCULATION         SPI_CRCCALCULATION_DISABLE

  // SPI Reset Pin: PA.8
#define BNRG_SPI_RESET_PIN              GPIO_PIN_8
#define BNRG_SPI_RESET_MODE             GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_RESET_PULL             GPIO_NOPULL
#define BNRG_SPI_RESET_SPEED            GPIO_SPEED_FREQ_LOW
#define BNRG_SPI_RESET_ALTERNATE        0
#define BNRG_SPI_RESET_PORT             GPIOA
#define BNRG_SPI_RESET_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define BNRG_SPI_RESET_STDBY_PIN        LL_PWR_GPIO_BIT_8
#define BNRG_SPI_RESET_STDBY_PORT       LL_PWR_GPIO_A

  // SCLK: 
#define BNRG_SPI_SCLK_PIN               GPIO_PIN_10
#define BNRG_SPI_SCLK_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_SCLK_PULL              GPIO_NOPULL
#define BNRG_SPI_SCLK_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_SCLK_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_SCLK_PORT              GPIOC
#define BNRG_SPI_SCLK_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

  // MISO (Master Input Slave Output): 
#define BNRG_SPI_MISO_PIN               GPIO_PIN_11
#define BNRG_SPI_MISO_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_MISO_PULL              GPIO_NOPULL
#define BNRG_SPI_MISO_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_MISO_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_MISO_PORT              GPIOC
#define BNRG_SPI_MISO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

  // MOSI (Master Output Slave Input): 
#define BNRG_SPI_MOSI_PIN               GPIO_PIN_12
#define BNRG_SPI_MOSI_MODE              GPIO_MODE_AF_PP
#define BNRG_SPI_MOSI_PULL              GPIO_NOPULL
#define BNRG_SPI_MOSI_SPEED             GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_MOSI_ALTERNATE         GPIO_AF6_SPI3
#define BNRG_SPI_MOSI_PORT              GPIOC
#define BNRG_SPI_MOSI_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define BNRG_SPI_MOSI_STDBY_PIN         LL_PWR_GPIO_BIT_12
#define BNRG_SPI_MOSI_STDBY_PORT        LL_PWR_GPIO_C

  // NSS/CSN/CS: PA.1
#define BNRG_SPI_CS_PIN                 GPIO_PIN_13
#define BNRG_SPI_CS_MODE                GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_CS_PULL                GPIO_NOPULL
#define BNRG_SPI_CS_SPEED               GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_CS_ALTERNATE           0
#define BNRG_SPI_CS_PORT                GPIOD
#define BNRG_SPI_CS_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()
#define BNRG_SPI_CS_STDBY_PIN           LL_PWR_GPIO_BIT_13
#define BNRG_SPI_CS_STDBY_PORT          LL_PWR_GPIO_D

/* IRQ PIN and PORT defined in the hw.h */
#define BNRG_SPI_IRQ_MODE               GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL               GPIO_PULLDOWN
#define BNRG_SPI_IRQ_SPEED              GPIO_SPEED_FREQ_VERY_HIGH
#define BNRG_SPI_IRQ_ALTERNATE          0
#define BNRG_SPI_IRQ_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()

/* EXTI PIN and PORT and Handler defined in the hw.h */
#define BNRG_SPI_EXTI_NVIC_PREEMPT_PRIO 2
#define BNRG_SPI_EXTI_NVIC_SUB_PRIO     0 

#define BNRG_SPI_FORCE_RESET()          __SPI3_FORCE_RESET()
#define BNRG_SPI_RELEASE_RESET()        __SPI3_RELEASE_RESET()

#define BNRG_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()
  /* Definition for SPIx's DMA */
#define BNRG_SPI_TX_DMA_ID                DMA2
#define BNRG_SPI_TX_DMA_CHANNEL           LL_DMA_CHANNEL_2
#define BNRG_SPI_TX_DMA_CHANNEL_INSTANCE  DMA2_Channel2
#define BNRG_SPI_TX_DMA_REQUEST           DMA_REQUEST_3
#define BNRG_SPI_TX_DMA_TC_FLAG           DMA_FLAG_TC2
#define BNRG_SPI_TX_DMA_IRQHandler        DMA2_Channel2_IRQHandler
#define BNRG_SPI_TX_DMA_NVIC_PREEMPT_PRIO 1
#define BNRG_SPI_TX_DMA_NVIC_SUB_PRIO     0

#define BNRG_SPI_RX_DMA_ID                DMA2
#define BNRG_SPI_RX_DMA_CHANNEL           LL_DMA_CHANNEL_1
#define BNRG_SPI_RX_DMA_CHANNEL_INSTANCE  DMA2_Channel1
#define BNRG_SPI_RX_DMA_REQUEST           DMA_REQUEST_3
#define BNRG_SPI_RX_DMA_TC_FLAG           DMA_FLAG_TC1
#define BNRG_SPI_RX_DMA_IRQHandler        DMA2_Channel1_IRQHandler
#define BNRG_SPI_RX_DMA_NVIC_PREEMPT_PRIO 1
#define BNRG_SPI_RX_DMA_NVIC_SUB_PRIO     0

  /* Definition for SPIx's NVIC */
#define BNRG_SPI_DMA_TX_IRQn            DMA2_Channel2_IRQn
#define BNRG_SPI_DMA_RX_IRQn            DMA2_Channel1_IRQn

#endif // _BLUENRG_TARGTES_H_
