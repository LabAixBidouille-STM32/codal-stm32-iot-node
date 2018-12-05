#pragma once

#include <stdint.h>
#include "stm32.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Sensors IO functions */
void     SENSOR_IO_Init(void);
void     SENSOR_IO_DeInit(void);
uint16_t SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t  SENSOR_IO_Read(uint8_t Addr, uint8_t Reg);
uint16_t SENSOR_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
uint16_t SENSOR_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
HAL_StatusTypeDef SENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
void     SENSOR_IO_Delay(uint32_t Delay);
uint32_t SENSOR_IO_Error();

#ifdef __cplusplus
}
#endif