#ifndef STM32_IOT_NODE_HUMIDITY_H
#define STM32_IOT_NODE_HUMIDITY_H

#include "CodalConfig.h"
#include "codal-core/inc/driver-models/Sensor.h"

#include "stm32l4xxPin.h"
#include "stm32l4xxI2C.h"
#include "stm32l475e_iot01.h"

#include "hsensor.h"
#include "hts221.h"

namespace codal
{
  /**
   * Represents the humidity sensor on the STM32 IOT node.
   */
  class STM32IotNodeHumidity : public Sensor
  {
    HSENSOR_DrvTypeDef *hsensor_drv; 
    bool isInitialized;

    public:
    
    /**
     * Constructor.
     */
    STM32IotNodeHumidity();

    protected:

    /**
     * Configures the humidity for celsius range and sample rate defined
     * in this object. The nearest values are chosen to those defined
     * that are supported by the hardware. The instance variables are then
     * updated to reflect reality.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the humidity could not be configured.
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
