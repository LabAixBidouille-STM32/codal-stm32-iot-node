#ifndef STM32_IOT_NODE_MAGNETOMETER_H
#define STM32_IOT_NODE_MAGNETOMETER_H

#include "CodalConfig.h"
#include "codal-core/inc/driver-models/Compass.h"

#include "stm32l4xxPin.h"
#include "stm32l4xxI2C.h"
#include "stm32l475e_iot01.h"

#include "magneto.h"
#include "lis3mdl.h"

namespace codal
{
  /**
   * Represents the accelerometer on the STM32 IOT node.
   */
 class STM32IotNodeMagnetometer : public Compass
 {
   MAGNETO_DrvTypeDef* magnetoDrv;
   bool isInitialized;
   CODAL_TIMESTAMP previousSampleTime;
   public:
   /**
    * Constructor.
    */
    STM32IotNodeMagnetometer(CoordinateSpace& coordinateSpace);

    virtual int setPeriod(int period);

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

    int updateSample();

    private:
    uint8_t getBestAdaptedODRValue();
    /*
     * Event Handler for periodic sample timer
     */
    void onSampleEvent(Event);
    };
}

#endif
