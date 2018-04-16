/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University, UK.
Copyright (c) 2018 Paul ADAM, Europe.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef STM32_IOT_NODE_ACCEL_H
#define STM32_IOT_NODE_ACCEL_H

#include "CodalConfig.h"
#include "Accelerometer.h"
#include "MbedPin.h"

#include "STM32IotNodeI2C.h"

#include "LSM6DSL_ACC_GYRO_driver_HL.h"

namespace codal
{
  /**
   * Represents the accelerometer on the STM32 IOT node.
   */
 class STM32IotNodeAccelerometer : protected codal::Accelerometer
 {
   LSM6DSL_X_Data_t LSM6DSL_X_Data =
   {
    &LSM6DSL_Combo_Data[ 0 ],
    0.0,
   };

   ACCELERO_Data_t ACCELERO_Data =
   {
    ( void * ) &LSM6DSL_X_Data,
    0,
   };

   DrvContextTypeDef DrvContext =
   {
    LSM6DSL_ACC_GYRO_WHO_AM_I,
    0,
    LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,
    0,
    0,
    0,
    0,
    0,
    &ACCELERO_Data,
    ( void * ) &LSM6DSL_X_Drv,
    0,
   };

   STM32IotNodeI2C& _i2c;

   public:
   /**
    * Constructor.
    */
    STM32IotNodeAccelerometer( STM32IotNodeI2C& i2c, codal::CoordinateSpace& coordinateSpace );

    using codal::Accelerometer::getPeriod;
    using codal::Accelerometer::getRange;
    using codal::Accelerometer::getSample;
    using codal::Accelerometer::getX;
    using codal::Accelerometer::getY;
    using codal::Accelerometer::getZ;
    using codal::Accelerometer::getPitch;
    using codal::Accelerometer::getPitchRadians;
    using codal::Accelerometer::getRoll;
    using codal::Accelerometer::getRollRadians;

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

    };
}

#endif
