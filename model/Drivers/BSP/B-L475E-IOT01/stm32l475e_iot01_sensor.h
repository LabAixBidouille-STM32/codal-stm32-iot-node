#pragma once

/* Sensors IO functions */
WEAK void     SENSOR_IO_Init(void);
WEAK void     SENSOR_IO_DeInit(void);
WEAK void     SENSOR_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
WEAK uint8_t  SENSOR_IO_Read(uint8_t Addr, uint8_t Reg);
WEAK uint16_t SENSOR_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
WEAK void     SENSOR_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
WEAK HAL_StatusTypeDef SENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
WEAK void     SENSOR_IO_Delay(uint32_t Delay);