/**
 ******************************************************************************
 * @file    VL53L0X_Driver_HL.h
 * @author  Paul ADAM
 * @version V0.0.1
 * @date    01-February-2018
 * @brief   This file contains definitions for the VL53L0X_Driver_HL.c firmware driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 Paul ADAM</center></h2>
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VL53L0X_DRIVER_HL_H
#define __VL53L0X_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include "distance.h"

/* Include sensor component drivers. */
//#include "VL53L0X_Driver.h"



/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENTS COMPONENTS
 * @{
 */

/** @addtogroup VL53L0X VL53L0X
 * @{
 */

/** @addtogroup VL53L0X_Public_Constants Public constants
 * @{
 */

#define VL53L0X_SENSORS_MAX_NUM  1     /**< VL53L0X max number of instances */

/** @addtogroup VL53L0X_I2C_Addresses VL53L0X I2C Addresses
 * @{
 */

#define VL53L0X_ADDRESS_DEFAULT  0x52  /**< VL53L0X I2C Address */

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup VL53L0X_Public_Types VL53L0X Public Types
 * @{
 */

/**
 * @brief VL53L0X combo specific data internal structure definition
 */

typedef struct
{
  uint8_t isInitialized;
  uint8_t isEnabled;
} VL53L0X_Combo_Data_t;

/**
 * @brief VL53L0X humidity specific data internal structure definition
 */

typedef struct
{
  VL53L0X_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} VL53L0X_H_Data_t;


/**
 * @brief VL53L0X temperature specific data internal structure definition
 */

typedef struct
{
  VL53L0X_Combo_Data_t *comboData;       /* Combo data to manage in software enable/disable of the combo sensors */
} VL53L0X_T_Data_t;

/**
 * @}
 */

/** @addtogroup VL53L0X_Public_Variables Public variables
 * @{
 */

extern DISTANCE_Drv_t VL53L0X_Drv;
extern VL53L0X_Combo_Data_t VL53L0X_Combo_Data[VL53L0X_SENSORS_MAX_NUM];

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __VL53L0X_DRIVER_HL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
