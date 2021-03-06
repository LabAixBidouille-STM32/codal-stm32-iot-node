/**
  ******************************************************************************
  * @file    stm32l475e_iot01.c
  * @author  MCD Application Team
  * @brief   STM32L475E-IOT01 board support package
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01.h"
#include <string.h>

/** @defgroup BSP BSP
  * @{
  */ 

/** @defgroup STM32L475E_IOT01 STM32L475E_IOT01
  * @{
  */

/** @defgroup STM32L475E_IOT01_LOW_LEVEL LOW LEVEL
  * @{
  */

/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Private_Defines LOW LEVEL Private Def
  * @{
  */
/**
 * @brief STM32L475E IOT01 BSP Driver version number
   */
#define __STM32L475E_IOT01_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32L475E_IOT01_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32L475E_IOT01_BSP_VERSION_SUB2   (0x01) /*!< [15:8]  sub2 version */
#define __STM32L475E_IOT01_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32L475E_IOT01_BSP_VERSION        ((__STM32L475E_IOT01_BSP_VERSION_MAIN << 24)\
                                                 |(__STM32L475E_IOT01_BSP_VERSION_SUB1 << 16)\
                                                 |(__STM32L475E_IOT01_BSP_VERSION_SUB2 << 8 )\
                                                 |(__STM32L475E_IOT01_BSP_VERSION_RC))
/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Private_Variables LOW LEVEL Variables 
  * @{
  */

const uint32_t GPIO_PIN[LEDn] = {LED2_PIN};


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED2_GPIO_PORT};


GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT};

const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN};

const uint16_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {DISCOVERY_COM1};

GPIO_TypeDef* COM_TX_PORT[COMn] = {DISCOVERY_COM1_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {DISCOVERY_COM1_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn] = {DISCOVERY_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {DISCOVERY_COM1_RX_PIN};

const uint16_t COM_TX_AF[COMn] = {DISCOVERY_COM1_TX_AF};

const uint16_t COM_RX_AF[COMn] = {DISCOVERY_COM1_RX_AF};

I2C_HandleTypeDef hI2cHandler;
UART_HandleTypeDef hDiscoUart;

/**
  * @}
  */
/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Private_FunctionPrototypes LOW LEVEL Private Function Prototypes
  * @{
  */
void     I2Cx_MspInit(I2C_HandleTypeDef *i2c_handler);
void     I2Cx_MspDeInit(I2C_HandleTypeDef *i2c_handler);
void     I2Cx_Init(I2C_HandleTypeDef *i2c_handler);
void     I2Cx_DeInit(I2C_HandleTypeDef *i2c_handler);
HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef *i2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
HAL_StatusTypeDef I2Cx_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials);
void              I2Cx_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr);

void     NFC_IO_Init(uint8_t GpoIrqEnable);
void     NFC_IO_DeInit(void);
uint16_t NFC_IO_ReadMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length );
uint16_t NFC_IO_WriteMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length);
uint16_t NFC_IO_IsDeviceReady (uint8_t Addr, uint32_t Trials);
void     NFC_IO_ReadState(uint8_t * pPinState);
void     NFC_IO_RfDisable(uint8_t PinState);
void     NFC_IO_Delay(uint32_t Delay);

/**
  * @}
  */

/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Private_Functions LOW LEVEL Private Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32L475E IOT01 BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32L475E_IOT01_BSP_VERSION;
}

/**
  * @brief  Configures LEDs.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED2
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  LEDx_GPIO_CLK_ENABLE(Led);
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin   = GPIO_PIN[Led];
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &gpio_init_structure);
}

/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED2
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = GPIO_PIN[Led];
  
  /* Turn off LED */
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
  HAL_GPIO_DeInit(GPIO_PORT[Led], gpio_init_structure.Pin);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: LED to be set on 
  *          This parameter can be one of the following values:
  *            @arg  LED2
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off. 
  * @param  Led: LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED2
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED2
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button
  * @param  ButtonMode: Button mode
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *            @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line 
  *                                    with interrupt generation capability  
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable the BUTTON clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Pin = BUTTON_PIN[Button];
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
  }
  
  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Pin = BUTTON_PIN[Button];
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;
    
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button 
  * @note PB DeInit does not disable the GPIO clock
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}


/**
  * @brief  Returns the selected button state.
  * @param  Button: Button to be checked
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button 
  * @retval The Button GPIO pin value (GPIO_PIN_RESET = button pressed)
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1 
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable GPIO clock */
  DISCOVERY_COMx_TX_GPIO_CLK_ENABLE(COM);
  DISCOVERY_COMx_RX_GPIO_CLK_ENABLE(COM);

  /* Enable USART clock */
  DISCOVERY_COMx_CLK_ENABLE(COM);

  /* Configure USART Tx as alternate function */
  gpio_init_structure.Pin = COM_TX_PIN[COM];
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Alternate = COM_TX_AF[COM];
  HAL_GPIO_Init(COM_TX_PORT[COM], &gpio_init_structure);

  /* Configure USART Rx as alternate function */
  gpio_init_structure.Pin = COM_RX_PIN[COM];
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = COM_RX_AF[COM];
  HAL_GPIO_Init(COM_RX_PORT[COM], &gpio_init_structure);

  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_Init(huart);
}

/**
  * @brief  DeInit COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1 
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
void BSP_COM_DeInit(COM_TypeDef COM, UART_HandleTypeDef *huart)
{
  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_DeInit(huart);

  /* Enable USART clock */
  DISCOVERY_COMx_CLK_DISABLE(COM);

  /* DeInit GPIO pins can be done in the application 
     (by surcharging this __weak function) */

  /* GPIO pins clock, FMC clock and DMA clock can be shut down in the application 
     by surcharging this __weak function */ 
}


/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/


/**
  * @}
  */



/******************************** LINK NFC ********************************/

/**
  * @brief  Initializes Sensors low level.
  * @param  GpoIrqEnable: 0x0 is disable, otherwise enabled  
  * @retval None
  */
void NFC_IO_Init(uint8_t GpoIrqEnable)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* I2C init */
  I2Cx_Init(&hI2cHandler);
  
  /* GPIO Ports Clock Enable */
  NFC_GPIO_CLK_ENABLE();
  
  /* Configure GPIO pins for GPO (PE4) */
  if(GpoIrqEnable == 0)
  {
    GPIO_InitStruct.Pin = NFC_GPIO_GPO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(NFC_GPIO_GPO_PIN_PORT, &GPIO_InitStruct);
  }
  else
  {
    GPIO_InitStruct.Pin = NFC_GPIO_GPO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(NFC_GPIO_GPO_PIN_PORT, &GPIO_InitStruct);
    /* Enable and set EXTI4_IRQn Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI4_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);    
  }
  
  /* Configure GPIO pins for DISABLE (PE2)*/
  GPIO_InitStruct.Pin = NFC_GPIO_RFDISABLE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NFC_GPIO_RFDISABLE_PIN_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes Sensors low level.
  * @retval None
  */
void NFC_IO_DeInit(void)
{
  I2Cx_DeInit(&hI2cHandler);
}

/**
  * @brief  This functions reads a response of the M24SR device
  * @param  Addr: M24SR I2C address   (do we really need to add?)
  * @param  pBuffer : Pointer on the buffer to retrieve M24SR response
  * @param  Length: Length of the data
  * @retval Status: Success or Timeout
  */
uint16_t NFC_IO_ReadMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length )
{
  uint16_t status ;
    
    /* Before calling this function M24SR must be ready: check to detect potential issues */
  status = NFC_IO_IsDeviceReady(Addr, NFC_I2C_TRIALS);
  if (status != NFC_I2C_STATUS_SUCCESS)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  } 
    
  if( HAL_I2C_Master_Receive(&hI2cHandler, Addr, (uint8_t*)pBuffer, Length, NFC_I2C_TIMEOUT_STD) != HAL_OK)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }
    
  return NFC_I2C_STATUS_SUCCESS;    
}

/**
  * @brief  This functions sends the command buffer 
  * @param  Addr: M24SR I2C address   (do we really need to add?)
  * @param  pBuffer : pointer to the buffer to send to the M24SR
  * @param  Length: Length of the data
  * @retval Status: Success or Timeout
  */
uint16_t  NFC_IO_WriteMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length) 
{
  uint16_t status ;
    
  /* Before calling this function M24SR must be ready: check to detect potential issues */
  status = NFC_IO_IsDeviceReady(Addr, NFC_I2C_TRIALS);
  if (status != NFC_I2C_STATUS_SUCCESS)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  } 
    
  if( HAL_I2C_Master_Transmit(&hI2cHandler, Addr, (uint8_t*)pBuffer, Length, NFC_I2C_TIMEOUT_STD) != HAL_OK)    
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }

  return NFC_I2C_STATUS_SUCCESS;
}

/**
  * @brief  Checks if target device is ready for communication.
  * @param  Addr: M24SR I2C address   (do we really need to add?)
  * @param  Trials: Number of trials (currently not present in M24sr)
  * @retval Status: Success or Timeout
  */
uint16_t   NFC_IO_IsDeviceReady (uint8_t Addr, uint32_t Trials)
{
    HAL_StatusTypeDef status;
    uint32_t tickstart = 0;
    uint32_t currenttick = 0;
    
    /* Get tick */
    tickstart = HAL_GetTick();
    
    /* Wait until M24SR is ready or timeout occurs */
    do
    {
        status = HAL_I2C_IsDeviceReady(&hI2cHandler, Addr, Trials, NFC_I2C_TIMEOUT_STD);
        currenttick = HAL_GetTick();
    } while( ( (currenttick - tickstart) < NFC_I2C_TIMEOUT_MAX) && (status != HAL_OK) );
    
    if (status != HAL_OK)
    {
        return NFC_I2C_ERROR_TIMEOUT;
    } 

    return NFC_I2C_STATUS_SUCCESS;
}

/**
  * @brief  This function read the state of the M24SR GPO
  * @param  none
  * @retval GPIO_PinState : state of the M24SR GPO
  */
void NFC_IO_ReadState(uint8_t * pPinState)
{
  *pPinState = (uint8_t)HAL_GPIO_ReadPin(NFC_GPIO_GPO_PIN_PORT,NFC_GPIO_GPO_PIN);
}

/**
  * @brief  This function set the state of the M24SR RF disable pin
  * @param  PinState: put RF disable pin of M24SR in PinState (1 or 0)
  */
void NFC_IO_RfDisable(uint8_t PinState)
{
  HAL_GPIO_WritePin(NFC_GPIO_RFDISABLE_PIN_PORT,NFC_GPIO_RFDISABLE_PIN,(GPIO_PinState)PinState);
}

/**
  * @brief  This function wait the time given in param (in milisecond)
  * @param  Delay: Delay in ms
  * @retval None
  */
void NFC_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
