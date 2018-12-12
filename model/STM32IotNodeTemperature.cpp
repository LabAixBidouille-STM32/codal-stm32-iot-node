/**
  * Class definition for temperature.
  * Represents the temperature on the STM IOT node.
  */

#include "CodalConfig.h"
#include "hts221_odr.h"
#include "STM32IotNode.h"
#include "STM32IotNodeTemperature.h"

namespace codal
{
  /**
    * Constructor.
    *
    * Create a representation of the temperature on the STM32 IOT node
    *
    */
  STM32IotNodeTemperature::STM32IotNodeTemperature()
    : Sensor(DEVICE_ID_THERMOMETER),
      tsensor_drv(&HTS221_T_Drv),
      isInitialized(false)
  {
    configure();
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

  int STM32IotNodeTemperature::configure()
  {
    /* Low level init */
    SENSOR_IO_Init();
    /* TSENSOR Init */   
    tsensor_drv->Init(HTS221_I2C_ADDRESS, NULL);
    
    if ( !samplePeriod )
      samplePeriod = 1;

    float frequency = 1000.0f / (float) samplePeriod;

    uint8_t odr = getBestAdaptedODRValue(frequency); 
    updateODR(odr);

    samplePeriod = 1000.0f / frequency;

    isInitialized = true;
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

  int STM32IotNodeTemperature::readValue()
  {
    if(!isInitialized)
      configure();
    return (int) (tsensor_drv->ReadTemp(TSENSOR_I2C_ADDRESS));
  }
}
