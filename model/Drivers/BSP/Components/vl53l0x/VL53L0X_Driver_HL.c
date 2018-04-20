/**
 ******************************************************************************
 * @file    VL53L0X_Driver_HL.c
 * @author  Paul ADAM
 * @version V0.0.1
 * @date    01-February-2018
 * @brief   This file provides a set of high-level functions needed to manage
            the VL53L0X sensor
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

/* Includes ------------------------------------------------------------------*/
#include "VL53L0X_Driver_HL.h"
#include <math.h>
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENTS COMPONENTS
 * @{
 */

/** @addtogroup VL53L0X VL53L0X
 * @{
 */

/** @addtogroup VL53L0X_Callable_Private_FunctionPrototypes Callable private function prototypes
 * @{
 */

static DrvStatusTypeDef VL53L0X_Init( DrvContextTypeDef *handle );
static DrvStatusTypeDef VL53L0X_DeInit( DrvContextTypeDef *handle );
static DrvStatusTypeDef VL53L0X_Sensor_Enable( DrvContextTypeDef *handle );
static DrvStatusTypeDef VL53L0X_Sensor_Disable( DrvContextTypeDef *handle );
//static DrvStatusTypeDef VL53L0X_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i );
static DrvStatusTypeDef VL53L0X_Check_WhoAmI( DrvContextTypeDef *handle );
static DrvStatusTypeDef VL53L0X_Get_Dist( DrvContextTypeDef *handle, float *distance );
//static DrvStatusTypeDef VL53L0X_Get_ODR( DrvContextTypeDef *handle, float *odr );
//static DrvStatusTypeDef VL53L0X_Set_ODR( DrvContextTypeDef *handle, SensorOdr_t odr );
//static DrvStatusTypeDef VL53L0X_Set_ODR_Value( DrvContextTypeDef *handle, float odr );
//static DrvStatusTypeDef VL53L0X_Read_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t *data );
//static DrvStatusTypeDef VL53L0X_Write_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t data );
//static DrvStatusTypeDef VL53L0X_Get_DRDY_Status( DrvContextTypeDef *handle, uint8_t *status );

/**
 * @}
 */

/** @addtogroup VL53L0X_Private_Function_Prototypes Private function prototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup VL53L0X_Private_Variables Private variables
 * @{
 */

/**
 * @brief VL53L0X distance driver structure
 */
DISTANCE_Drv_t VL53L0X_Drv =
{
  VL53L0X_Init,
  VL53L0X_DeInit,
  VL53L0X_Sensor_Enable,
  VL53L0X_Sensor_Disable,
//  VL53L0X_Get_WhoAmI,
  VL53L0X_Check_WhoAmI,
  VL53L0X_Get_Dist,
//  VL53L0X_Get_ODR,
//  VL53L0X_Set_ODR,
//  VL53L0X_Set_ODR_Value,
//  VL53L0X_Read_Reg,
//  VL53L0X_Write_Reg,
//  VL53L0X_Get_DRDY_Status
};

/**
 * @brief VL53L0X combo data structure definition
 */
VL53L0X_Combo_Data_t VL53L0X_Combo_Data[VL53L0X_SENSORS_MAX_NUM];

/**
 * @}
 */

/** @addtogroup VL53L0X_Callable_Private_Functions Callable private functions
 * @{
 */

/**
 * @brief Initialize the VL53L0X sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_Init( DrvContextTypeDef *handle )
{
 if ( VL53L0X_Check_WhoAmI( handle ) == COMPONENT_ERROR )
 {
   return COMPONENT_ERROR;
 }

 uint8_t  VhvSettings;
 uint8_t  PhaseCal;
 uint32_t refSpadCount;
 uint8_t  isApertureSpads;
 FixPoint1616_t signalLimit = (FixPoint1616_t)(0.25*65536);
 FixPoint1616_t sigmaLimit  = (FixPoint1616_t)(18*65536);
 uint32_t timingBudget = 33000;
 uint8_t preRangeVcselPeriod = 14;
 uint8_t finalRangeVcselPeriod = 10;
 VL53L0X_Dev_t* Dev = ( VL53L0X_Dev_t* ) handle->pData;
 Dev->Present = 1;
 {
  if ( VL53L0X_DataInit( Dev ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_StaticInit( Dev ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_PerformRefCalibration( Dev, &VhvSettings, &PhaseCal ) )
  {
//   return COMPONENT_ERROR;
  }
  if ( VL53L0X_PerformRefSpadManagement( Dev, &refSpadCount, &isApertureSpads ) )
  {
//   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetDeviceMode( Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetLimitCheckEnable( Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1 ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetLimitCheckEnable( Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1 ) )
  {
   return COMPONENT_ERROR;
  }
  /* Ranging configuration */
  signalLimit  = ( FixPoint1616_t ) ( 0.25 * 65536 );
  sigmaLimit   = ( FixPoint1616_t ) ( 18  * 65536 );
  timingBudget = 200000;
  preRangeVcselPeriod   = 18;
  finalRangeVcselPeriod = 14;

  if ( VL53L0X_SetLimitCheckValue( Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signalLimit ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetLimitCheckValue( Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigmaLimit ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetMeasurementTimingBudgetMicroSeconds( Dev, timingBudget ) )
  {
   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetVcselPulsePeriod( Dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, preRangeVcselPeriod ) )
  {
//   return COMPONENT_ERROR;
  }
  if ( VL53L0X_SetVcselPulsePeriod( Dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, finalRangeVcselPeriod ) )
  {
//   return COMPONENT_ERROR;
  }

  Dev->LeakyFirst=1;
 }


//  /* Power down the device */
//  if ( VL53L0X_DeActivate( (void *)handle ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//
//  /* Enable BDU */
//  if ( VL53L0X_Set_BduMode( (void *)handle, VL53L0X_ENABLE ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//
//  /* Set default ODR */
//  if ( VL53L0X_Set_ODR( handle, ODR_LOW ) == COMPONENT_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }

  handle->isInitialized = 1;

  return COMPONENT_OK;
}

/**
 * @brief Deinitialize the VL53L0X distance sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_DeInit( DrvContextTypeDef *handle )
{

  /* Check if the VL53L0X humidity sensor is already initialized. */
  /* If yes, skip the deinitialize function, if not call deinitialize function */
//  if((((VL53L0X_T_Data_t *)(((DISTANCE_Data_t *)(handle->pData))->pComponentData))->comboData->isHumInitialized == 0))
//  {
//    if(VL53L0X_T_Sensor_Disable(handle) == COMPONENT_ERROR)
//    {
//      return COMPONENT_ERROR;
//    }
//  }
//
//  ((VL53L0X_T_Data_t *)(((DISTANCE_Data_t *)(handle->pData))->pComponentData))->comboData->isTempInitialized = 0;

  handle->isInitialized = 0;

  return COMPONENT_OK;
}


/**
 * @brief Enable the VL53L0X distance sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_Sensor_Enable( DrvContextTypeDef *handle )
{

//  /* Check if the component is already enabled */
//  if ( handle->isEnabled == 1 )
//  {
//    return COMPONENT_OK;
//  }
//
//  /* Power up the device */
//  if ( VL53L0X_Activate( (void *)handle ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }

  handle->isEnabled = 1;

  return COMPONENT_OK;
}


/**
 * @brief Disable the VL53L0X distance sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_Sensor_Disable( DrvContextTypeDef *handle )
{

  /* Check if the component is already disabled */
//  if ( handle->isEnabled == 0 )
//  {
//    return COMPONENT_OK;
//  }
//
//  /* Power down the device */
//  if ( VL53L0X_DeActivate( (void *)handle ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }

  handle->isEnabled = 0;

  return COMPONENT_OK;
}

/**
 * @}
 */

/** @addtogroup VL53L0X_Private_Functions Private functions
 * @{
 */


/**
 * @brief Get the WHO_AM_I ID of the VL53L0X sensor
 * @param handle the device handle
 * @param who_am_i pointer to the value of WHO_AM_I register
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i )
//{
//  /* Read WHO AM I register */
//  if ( VL53L0X_Get_DeviceID( (void *)handle, who_am_i ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//  return COMPONENT_OK;
//}

/**
 * @brief Check the WHO_AM_I ID of the VL53L0X sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_Check_WhoAmI( DrvContextTypeDef *handle )
{
 uint16_t who_am_i = 0x00;

 if (VL53L0X_ERROR_NONE == VL53L0X_RdWord( handle->pData, VL53L0X_REG_IDENTIFICATION_MODEL_ID, (uint16_t *) &who_am_i))
 {
  if ( who_am_i == 0xEEAA )
  {
   return COMPONENT_OK;
  }
 }
 return COMPONENT_ERROR;
}

/**
 * @brief Get the distance value of the VL53L0X sensor
 * @param handle the device handle
 * @param distance pointer where the value is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef VL53L0X_Get_Dist( DrvContextTypeDef *handle, float *distance )
{

  /* Read data from VL53L0X. */
  VL53L0X_Dev_t* Dev = ( VL53L0X_Dev_t* ) handle->pData;
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_PerformSingleRangingMeasurement( Dev, &RangingMeasurementData );

  *distance = RangingMeasurementData.RangeMilliMeter < 3000 ? ( float ) RangingMeasurementData.RangeMilliMeter / 1000.0f : 3.0f;

  return COMPONENT_OK;
}

/**
 * @brief Get the VL53L0X sensor output data rate
 * @param handle the device handle
 * @param odr pointer where the output data rate is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Get_ODR( DrvContextTypeDef *handle, float *odr )
//{
//
//  VL53L0X_Odr_et odr_low_level;
//
//  if ( VL53L0X_Get_Odr( (void *)handle, &odr_low_level ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//
//  switch( odr_low_level )
//  {
//    case VL53L0X_ODR_ONE_SHOT:
//      *odr =  0.0f;
//      break;
//    case VL53L0X_ODR_1HZ     :
//      *odr =  1.0f;
//      break;
//    case VL53L0X_ODR_7HZ     :
//      *odr =  7.0f;
//      break;
//    case VL53L0X_ODR_12_5HZ  :
//      *odr = 12.5f;
//      break;
//    default                 :
//      *odr = -1.0f;
//      return COMPONENT_ERROR;
//  }
//
//  return COMPONENT_OK;
//}

/**
 * @brief Set the VL53L0X sensor output data rate
 * @param handle the device handle
 * @param odr the functional output data rate to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Set_ODR( DrvContextTypeDef *handle, SensorOdr_t odr )
//{
//
//  VL53L0X_Odr_et new_odr;
//
//  switch( odr )
//  {
//    case ODR_LOW:
//      new_odr = VL53L0X_ODR_1HZ;
//      break;
//    case ODR_MID_LOW:
//      new_odr = VL53L0X_ODR_12_5HZ;
//      break;
//    case ODR_MID:
//      new_odr = VL53L0X_ODR_12_5HZ;
//      break;
//    case ODR_MID_HIGH:
//      new_odr = VL53L0X_ODR_12_5HZ;
//      break;
//    case ODR_HIGH:
//      new_odr = VL53L0X_ODR_12_5HZ;
//      break;
//    default:
//      return COMPONENT_ERROR;
//  }
//
//  if ( VL53L0X_Set_Odr( (void *)handle, new_odr ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//
//  return COMPONENT_OK;
//}

/**
 * @brief Set the VL53L0X sensor output data rate
 * @param handle the device handle
 * @param odr the output data rate value to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Set_ODR_Value( DrvContextTypeDef *handle, float odr )
//{
//  VL53L0X_Odr_et new_odr;
//
//  new_odr = ( odr <= 1.0f ) ? VL53L0X_ODR_1HZ
//            : ( odr <= 7.0f ) ? VL53L0X_ODR_7HZ
//            :                   VL53L0X_ODR_12_5HZ;
//
//  if ( VL53L0X_Set_Odr( (void *)handle, new_odr ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//  return COMPONENT_OK;
//}

/**
 * @brief Read the data from register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Read_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t *data )
//{
//  if ( VL53L0X_ReadReg( (void *)handle, reg, 1, data ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//  return COMPONENT_OK;
//}

/**
 * @brief Write the data to register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef VL53L0X_Write_Reg( DrvContextTypeDef *handle, uint8_t reg, uint8_t data )
//{
//  if ( VL53L0X_WriteReg( (void *)handle, reg, 1, &data ) == VL53L0X_ERROR )
//  {
//    return COMPONENT_ERROR;
//  }
//  return COMPONENT_OK;
//}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
