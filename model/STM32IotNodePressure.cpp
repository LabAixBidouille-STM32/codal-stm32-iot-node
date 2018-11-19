/**
 * Class definition for pressure.
 * Represents the pressure on the STM IOT node.
 */

#include "CodalConfig.h"
#include "STM32IotNode.h"
#include "STM32IotNodePressure.h"

namespace codal
{
  /**
   * Constructor.
   *
   * Create a representation of the pressure on the STM32 IOT node
   *
   */
  STM32IotNodePressure::STM32IotNodePressure()
  :  Sensor(DEVICE_ID_PRESSURE), 
    psensor_drv(&LPS22HB_P_Drv),
    isInitialized(false)
  {
    configure();
  }

  /**
   * Configures output data rate register
   *
   *
   */
  static void updateODR(uint8_t odr){
    uint8_t tmp;
    /* Read CTRL_REG1 */
    tmp = SENSOR_IO_Read(LPS22HB_I2C_ADDRESS, LPS22HB_CTRL_REG1);
    
    /* Set default ODR */
    tmp &= ~LPS22HB_ODR_MASK;
    tmp |= (odr << 0x4) & LPS22HB_ODR_MASK; /*Set ODR*/
    
    /* Apply settings to CTRL_REG1 */
    SENSOR_IO_Write(LPS22HB_I2C_ADDRESS, LPS22HB_CTRL_REG1, tmp);
  }

  static uint8_t getBestAdaptedODRValue(float& frequency){
    uint8_t odr = 0; 

    if(frequency <= 5.f){
      odr = 1; //frequency = 1Hz
      frequency = 1.f;
    }
    else if (frequency <= 18.f){
      odr = 2; //frequency = 10Hz
      frequency = 10.f;
    }
    else if (frequency <= 38.f){
      odr = 3; //frequency = 25Hz
      frequency = 25.f;    
    }
    else if (frequency <= 63.f){
      odr = 4; //frequency = 50Hz
      frequency = 50.f;    
    }
    else if (frequency > 63.f){
      odr = 5; //frequency = 75Hz
      frequency = 75.f;    
    }
    else{
      odr = 0; //one shot
      frequency = 0;
    }
    return odr;

  }

  /**
   * Configures the pressure range defined
   * in this object. The nearest values are chosen to those defined
   * that are supported by the hardware. The instance variables are then
   * updated to reflect reality.
   *
   * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the pressure could not be configured.
   *
   */
  int STM32IotNodePressure::configure( )
  {
    int ret = DEVICE_OK;

    if(LPS22HB_P_Drv.ReadID(LPS22HB_I2C_ADDRESS) != LPS22HB_WHO_AM_I_VAL){
      ret = DEVICE_I2C_ERROR;
    }
    else{  
      psensor_drv->Init(LPS22HB_I2C_ADDRESS);
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
  int STM32IotNodePressure::readValue()
  {
    if(!isInitialized)
      configure();
    return (int) psensor_drv->ReadPressure(LPS22HB_I2C_ADDRESS);
  }

}
