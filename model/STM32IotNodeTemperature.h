#ifndef STM32_IOT_NODE_TEMP_H
#define STM32_IOT_NODE_TEMP_H

#include "CodalConfig.h"
#include "codal-core/inc/driver-models/Sensor.h"

#include "stm32l4xxPin.h"
#include "stm32l4xxI2C.h"
#include "stm32l475e_iot01.h"

#include "tsensor.h"
#include "hts221.h"

namespace codal
{
  /**
   * Represents the gyroscope on the STM32 IOT node.
   */
 class STM32IotNodeTemperature
 {
   STM32L4xxI2C& _i2c;
   TSENSOR_DrvTypeDef *tsensor_drv; 
   bool isInitialized;
   uint16_t samplePeriod;       // The time between samples, in milliseconds.
   public:

   /**
    * Constructor.
    */
    STM32IotNodeTemperature(STM32L4xxI2C& i2c);

    int getValue();

   protected:

    /**
     * Configures the temperature for celsuiu range and sample rate defined
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
