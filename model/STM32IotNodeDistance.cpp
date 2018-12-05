/**
  * Class definition for temperature.
  * Represents the temperature on the STM IOT node.
  */

#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "STM32IotNode.h"
#include "STM32IotNodeDistance.h"

using namespace codal;

/**
  * Constructor.
  *
  * Create a representation of the temperature on the STM32 IOT node
  *
  */
STM32IotNodeDistance::STM32IotNodeDistance()
: Sensor(DEVICE_ID_DISTANCE, SENSOR_DEFAULT_SENSITIVITY, 500),
  device()
{
  memset(&device, 0, sizeof(device));
  device.I2cHandle = default_i2c_sensors_bus->getHandle();
  device.I2cDevAddr = PROXIMITY_I2C_ADDRESS;

  GPIO_InitTypeDef GPIO_InitStruct;

  /*Configure GPIO pin : VL53L0X_XSHUT_Pin */
  GPIO_InitStruct.Pin = VL53L0X_XSHUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(VL53L0X_XSHUT_GPIO_Port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(VL53L0X_XSHUT_GPIO_Port, VL53L0X_XSHUT_Pin, GPIO_PIN_SET);
}

/**
 * Configures the temperature for celsius range defined
 * in this object. The nearest values are chosen to those defined
 * that are supported by the hardware. The instance variables are then
 * updated to reflect reality.
 *
 * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the temperature could not be configured.
 *
 */

int STM32IotNodeDistance::configure()
{
  printf("STM32IotNodeDistance::configure()\n");
  target_wait(1000);
  SENSOR_IO_Init(); 
  
  uint16_t vl53l0x_id = 0; 
  VL53L0X_DeviceInfo_t VL53L0X_DeviceInfo; 
  
  memset(&VL53L0X_DeviceInfo, 0, sizeof(VL53L0X_DeviceInfo_t));
  
  if (VL53L0X_ERROR_NONE == VL53L0X_GetDeviceInfo(&device, &VL53L0X_DeviceInfo))
  {  
    if (VL53L0X_ERROR_NONE == VL53L0X_RdWord(&device, VL53L0X_REG_IDENTIFICATION_MODEL_ID, (uint16_t *) &vl53l0x_id))
    {
      if (vl53l0x_id == VL53L0X_ID)
      {
        if (VL53L0X_ERROR_NONE == VL53L0X_DataInit(&device))
        {
          device.Present = 1;
          SetupSingleShot(device);
        }
        else
        { 
          printf("VL53L0X Time of Flight Failed to send its ID!\n");
          return DEVICE_I2C_ERROR;
        }
      }
    }
    else
    {
      printf("VL53L0X Time of Flight Failed to Initialize!\n");
      return DEVICE_I2C_ERROR;
    }
  }
  else
  {
    printf("VL53L0X Time of Flight Failed to get infos!\n");
    return DEVICE_I2C_ERROR;
  }  

  printf("Init sensor_vl53l0x ok...\n");
  return DEVICE_OK;
}

/**
 * Poll to see if new data is available from the hardware. If so, update it.
 * n.b. it is not necessary to explicitly call this function to update data
 * (it normally happens in the background when the scheduler is idle), but a check is performed
 * if the user explicitly requests up to date data.
 *
 * @return The value on success, DEVICE_I2C_ERROR if the update fails.
 *
 */

int STM32IotNodeDistance::readValue()
{
  init();
  uint16_t distance;
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  VL53L0X_PerformSingleRangingMeasurement(&device, &RangingMeasurementData);
  distance = RangingMeasurementData.RangeMilliMeter;
  return distance;
}
