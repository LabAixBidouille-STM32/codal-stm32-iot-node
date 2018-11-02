#ifndef STM32_IOT_NODE_ACCEL_H
#define STM32_IOT_NODE_ACCEL_H

#include "CodalConfig.h"
#include "codal-core/inc/driver-models/Accelerometer.h"

#include "stm32l4xxPin.h"
#include "stm32l4xxI2C.h"
#include "stm32l475e_iot01.h"

#include "accelero.h"
#include "lsm6dsl.h"

namespace codal
{
  /**
   * Represents the accelerometer on the STM32 IOT node.
   */
 class STM32IotNodeAccelerometer : protected Accelerometer
 {
   STM32L4xxI2C& _i2c;
   ACCELERO_DrvTypeDef* accelerometerDrv;
   bool isInitialized;
   public:
   /**
    * Constructor.
    */
    STM32IotNodeAccelerometer( STM32L4xxI2C& i2c, CoordinateSpace& coordinateSpace );

    using Accelerometer::getPeriod;
    using Accelerometer::getRange;
    using Accelerometer::getSample;
    using Accelerometer::getX;
    using Accelerometer::getY;
    using Accelerometer::getZ;
    using Accelerometer::getPitch;
    using Accelerometer::getPitchRadians;
    using Accelerometer::getRoll;
    using Accelerometer::getRollRadians;

   protected:

    /**
     * Configures the accelerometer for G range and sample rate defined
     * in this object. The nearest values are chosen to those defined
     * that are supported by the hardware. The instance variables are then
     * updated to reflect reality.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the accelerometer could not be configured.
     *
     * @note This method should be overidden by the hardware driver to implement the requested
     * changes in hardware.
     */
    virtual int configure();

    /**
     * Poll to see if new data is available from the hardware. If so, update it.
     * n.b. it is not necessary to explicitly call this function to update data
     * (it normally happens in the background when the scheduler is idle), but a check is performed
     * if the user explicitly requests up to date data.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
     *
     * @note This method should be overidden by the hardware driver to implement the requested
     * changes in hardware.
     */
    virtual int requestUpdate();

    private:
    uint8_t getBestAdaptedODRValue();
    uint8_t getBestAdaptedFSValue();
    };
}

#endif
