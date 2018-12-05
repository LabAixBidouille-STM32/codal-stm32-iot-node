#ifndef STM32_IOT_NODE_DISTANCE_H
#define STM32_IOT_NODE_DISTANCE_H

#include "CodalConfig.h"
#include "codal-core/inc/driver-models/Sensor.h"

#include "stm32l4xxPin.h"
#include "stm32l4xxI2C.h"
#include "stm32l475e_iot01.h"
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"
#include "vl53l0x_tof.h"

#define PROXIMITY_I2C_ADDRESS         ((uint16_t)0x0052)
#define VL53L0X_ID                    ((uint16_t)0xEEAA)
#define VL53L0X_XSHUT_Pin GPIO_PIN_6
#define VL53L0X_XSHUT_GPIO_Port GPIOC

namespace codal
{
  /**
   * Represents the gyroscope on the STM32 IOT node.
   */
 class STM32IotNodeDistance : public Sensor
 {
    VL53L0X_Dev_t device;
 
 public:
   /**
    * Constructor.
    */
    STM32IotNodeDistance();

    /**
     * Configures the distance for millimeter range and sample rate defined
     * in this object. The nearest values are chosen to those defined
     * that are supported by the hardware. The instance variables are then
     * updated to reflect reality.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the temperature could not be configured.
     *
     * @note This method should be overidden by the hardware driver to implement the requested
     * changes in hardware.
     */
    virtual int configure();

    /**
     * Read the value from underlying hardware.
     */
    virtual int readValue();

    };
}

#endif
