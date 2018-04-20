/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University, UK.
Copyright (c) 2018 Paul ADAM, inidinn.com

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

#ifndef STM32_IOT_NODE_DISTANCE_H
#define STM32_IOT_NODE_DISTANCE_H

#include "CodalConfig.h"
#include "Sensor.h"
#include "MbedPin.h"

#include "STM32IotNodeI2C.h"

#include "../vl53l0x/VL53L0X_Driver_HL.h"
#include "../vl53l0x/vl53l0x_def.h"
#include "../vl53l0x/vl53l0x_api.h"
#include "../vl53l0x/vl53l0x_tof.h"

namespace codal
{
  /**
   * Represents the gyroscope on the STM32 IOT node.
   */
 class STM32IotNodeDistance : public codal::Sensor
 {

//   VL53L0X_Dev_t VL53L0X_Dev =
//   {
//    .Data       = { 0 },
//    .I2cDevAddr = VL53L0X_ADDRESS_DEFAULT,
//   };
//
//   DrvContextTypeDef DrvContext =
//   {
//    .who_am_i      = 0xEE,
//    .ifType        = 0,
//    .address       = VL53L0X_ADDRESS_DEFAULT,
//    .spiDevice     = 0,
//    .instance      = 0,
//    .isInitialized = 0,
//    .isEnabled     = 0,
//    .isCombo       = 0,
//    .pData         = ( void * ) &VL53L0X_Dev,
//    .pVTable       = ( void * ) &VL53L0X_Drv,
//    .pExtVTable    = 0,
//   };

   VL53L0X_Dev_t VL53L0X_Dev;

   DrvContextTypeDef DrvContext;

   STM32IotNodeI2C& _i2c;

   public:
   /**
    * Constructor.
    */
    STM32IotNodeDistance( STM32IotNodeI2C& i2c );

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
