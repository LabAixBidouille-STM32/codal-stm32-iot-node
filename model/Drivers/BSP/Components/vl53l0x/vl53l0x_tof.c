/**
  ******************************************************************************
  * @file    vl53l0x_tof.c
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
#include <string.h>
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01_sensor.h"
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"

#include "vl53l0x_tof.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_TIME_OUT_BASE   10
#define I2C_TIME_OUT_BYTE   1

#define VL53L0X_OsDelay(...) HAL_Delay(2)
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    
    status_int = SENSOR_IO_ReadMultiple(Dev->I2cDevAddr, index, data, 1);

    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

uint8_t _I2CBuffer[64];


// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count) {
    int status_int;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    
    if (count > sizeof(_I2CBuffer) - 1) {
        return VL53L0X_ERROR_INVALID_PARAMS;
    }
    
    status_int = SENSOR_IO_WriteMultiple(Dev->I2cDevAddr, index, pdata, count);
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    
    return Status;
}

// the ranging_sensor_comms.dll will take care of the page selection
VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    
    status_int = SENSOR_IO_ReadMultiple(Dev->I2cDevAddr, index, pdata, count);    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    return Status;
}


VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;

    status_int = SENSOR_IO_ReadMultiple(Dev->I2cDevAddr, index, _I2CBuffer, 2);
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint16_t)_I2CBuffer[0]<<8) + (uint16_t)_I2CBuffer[1];
    done:
    return Status;
}

VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;

    status_int = SENSOR_IO_ReadMultiple(Dev->I2cDevAddr, index, _I2CBuffer, 4);
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint32_t)_I2CBuffer[0]<<24) + ((uint32_t)_I2CBuffer[1]<<16) + ((uint32_t)_I2CBuffer[2]<<8) + (uint32_t)_I2CBuffer[3];

done:
    return Status;
}

VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;

    status_int = SENSOR_IO_WriteMultiple(Dev->I2cDevAddr, index, &data, 1);;
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    
    return Status;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = data >> 8;
    _I2CBuffer[1] = data & 0x00FF;

    status_int = SENSOR_IO_WriteMultiple(Dev->I2cDevAddr, index, _I2CBuffer, 2);
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }
    
    return Status;
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV Dev, uint8_t index, uint8_t AndData, uint8_t OrData) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t data;

    Status = VL53L0X_RdByte(Dev, index, &data);
    
    if (Status) {
        goto done;
    }
    
    data = (data & AndData) | OrData;
    Status = VL53L0X_WrByte(Dev, index, data);
done:
    return Status;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data) {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    
    _I2CBuffer[0] = (data >> 24) & 0xFF;
    _I2CBuffer[1] = (data >> 16) & 0xFF;
    _I2CBuffer[2] = (data >> 8)  & 0xFF;
    _I2CBuffer[3] = (data >> 0 ) & 0xFF;

    status_int = SENSOR_IO_WriteMultiple(Dev->I2cDevAddr, index, _I2CBuffer, 4);
    
    if (status_int != 0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev) {
    VL53L0X_Error status = VL53L0X_ERROR_NONE;

    // do nothing
    VL53L0X_OsDelay();
    return status;
}

/**
 *  Setup all detected sensors for single shot mode and setup ranging configuration
 */
void SetupSingleShot(VL53L0X_Dev_t Dev)
{
  int status;
  uint8_t VhvSettings;
  uint8_t PhaseCal;
  uint32_t refSpadCount;
	uint8_t isApertureSpads;
	FixPoint1616_t signalLimit = (FixPoint1616_t)(0.25*65536);
	FixPoint1616_t sigmaLimit = (FixPoint1616_t)(18*65536);
	uint32_t timingBudget = 33000;
	uint8_t preRangeVcselPeriod = 14;
	uint8_t finalRangeVcselPeriod = 10;

                          
  if( Dev.Present){
    status=VL53L0X_StaticInit(&Dev);
    if( status ){
      printf("VL53L0X_StaticInit failed\n");
    }
    
    
    status = VL53L0X_PerformRefCalibration(&Dev, &VhvSettings, &PhaseCal);
    if( status ){
      printf("VL53L0X_PerformRefCalibration failed\n");
    }
    
    status = VL53L0X_PerformRefSpadManagement(&Dev, &refSpadCount, &isApertureSpads);
    if( status ){
      printf("VL53L0X_PerformRefSpadManagement failed\n");
    }
    
    status = VL53L0X_SetDeviceMode(&Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if( status ){
      printf("VL53L0X_SetDeviceMode failed\n");
    }
    
    status = VL53L0X_SetLimitCheckEnable(&Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); // Enable Sigma limit
    if( status ){
      printf("VL53L0X_SetLimitCheckEnable failed\n");
    }
    
    status = VL53L0X_SetLimitCheckEnable(&Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); // Enable Signa limit
    if( status ){
      printf("VL53L0X_SetLimitCheckEnable failed\n");
    }
    
    /* Ranging configuration */
    signalLimit = (FixPoint1616_t)(0.1*65536);
    sigmaLimit = (FixPoint1616_t)(60*65536);
    timingBudget = 33000;
    preRangeVcselPeriod = 18;
    finalRangeVcselPeriod = 14;
    
    status = VL53L0X_SetLimitCheckValue(&Dev,  VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signalLimit);
    
    if( status ){
      printf("VL53L0X_SetLimitCheckValue failed\n");
    }
    
    status = VL53L0X_SetLimitCheckValue(&Dev,  VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigmaLimit);
    if( status ){
      printf("VL53L0X_SetLimitCheckValue failed\n");
    }
    
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(&Dev,  timingBudget);
    if( status ){
      printf("VL53L0X_SetMeasurementTimingBudgetMicroSeconds failed\n");
    }
    
    status = VL53L0X_SetVcselPulsePeriod(&Dev,  VL53L0X_VCSEL_PERIOD_PRE_RANGE, preRangeVcselPeriod);
    if( status ){
      printf("VL53L0X_SetVcselPulsePeriod failed\n");
    }
    
    status = VL53L0X_SetVcselPulsePeriod(&Dev,  VL53L0X_VCSEL_PERIOD_FINAL_RANGE, finalRangeVcselPeriod);
    if( status ){
      printf("VL53L0X_SetVcselPulsePeriod failed\n");
    }
    
    Dev.LeakyFirst=1;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
