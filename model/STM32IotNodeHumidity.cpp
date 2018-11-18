/**
  * Class definition for humidity.
  * Represents the humidity on the STM IOT node.
  */

#include "CodalConfig.h"
#include "hts221_odr.h"
#include "STM32IotNode.h"
#include "STM32IotNodeHumidity.h"

namespace codal
{
  /**
    * Constructor.
    *
    * Create a representation of the humidity on the STM32 IOT node
    *
    */
  STM32IotNodeHumidity::STM32IotNodeHumidity()
  :  Sensor(DEVICE_ID_HUMIDITY), 
    hsensor_drv(&HTS221_H_Drv),
    isInitialized(false)
  {
    configure();
  }

  /**
  * Configures the humidity range defined
  * in this object. The nearest values are chosen to those defined
  * that are supported by the hardware. The instance variables are then
  * updated to reflect reality.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the humidity could not be configured.
  *
  */

  int STM32IotNodeHumidity::configure()
  {
    int ret = DEVICE_OK;
    
    if(hsensor_drv->ReadID(HTS221_I2C_ADDRESS) != HTS221_WHO_AM_I_VAL){
      ret = DEVICE_I2C_ERROR;
    }
    else{ 
      hsensor_drv->Init(HTS221_I2C_ADDRESS);
    }
    
    if ( !samplePeriod )
      samplePeriod = 1;

    float frequency = 1000.0f / (float) samplePeriod;

    uint8_t odr = getBestAdaptedODRValue(frequency); 
    updateODR(odr);

    samplePeriod = 1000.0f / frequency;

    isInitialized = true;
    return ret;
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

  int STM32IotNodeHumidity::readValue()
  {
    if(!isInitialized)
      configure();
    return (int) hsensor_drv->ReadHumidity(HTS221_I2C_ADDRESS) * 10;
  }

}
