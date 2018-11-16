/**
  * Class definition for temperature.
  * Represents the temperature on the STM IOT node.
  */

#include "CodalConfig.h"
#include "STM32IotNode.h"
#include "STM32IotNodeTemperature.h"
namespace codal
{

/*
 * Event Handler for periodic sample timer
 */
  void onSampleEvent(Event){

  }
  /**
    * Constructor.
    *
    * Create a representation of the temperature on the STM32 IOT node
    *
    */
  STM32IotNodeTemperature::STM32IotNodeTemperature( STM32L4xxI2C& i2c )
  :  
    _i2c(i2c),
    tsensor_drv(&HTS221_T_Drv),
    isInitialized(false)
  {
        // Configure for a 2 Hz update frequency by default.
    if(EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(DEVICE_ID_THERMOMETER, SENSOR_UPDATE_NEEDED, onSampleEvent, MESSAGE_BUS_LISTENER_IMMEDIATE);
    readValue();
    //configure();
    //updateSample();
  }

  /**
  * Configures output data rate register
  *
  *
  */
  void updateODR(uint8_t odr){
    uint8_t tmp;
    /* Read CTRL_REG1 */
    tmp = SENSOR_IO_Read(TSENSOR_I2C_ADDRESS, HTS221_CTRL_REG1);
    
    /* Set default ODR */
    tmp &= ~HTS221_ODR_MASK;
    tmp |= odr & HTS221_ODR_MASK; /*Set ODR*/
    
    /* Apply settings to CTRL_REG1 */
    SENSOR_IO_Write(TSENSOR_I2C_ADDRESS, HTS221_CTRL_REG1, tmp);
  }

  uint8_t getBestAdaptedODRValue(float& frequency){
  uint8_t odr = 0; 

    if(frequency <= 5.f){
      odr = 1; //frequency = 1Hz
      frequency = 1.f;
    }
    else if (frequency <= 10.f){
      odr = 2; //frequency = 7Hz
      frequency = 7.f;
    }
    else if (frequency > 10.f){
      odr = 3; //frequency = 12.5Hz
      frequency = 12.5f;    
    }
    else{
      odr = 0; //one shot
      frequency = 0;
    }
    return odr;

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
    return (int) tsensor_drv->ReadTemp(TSENSOR_I2C_ADDRESS) * 10;
  }

}
