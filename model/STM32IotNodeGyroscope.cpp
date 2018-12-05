/**
  * Class definition for accelerometer.
  * Represents the accelerometer on the STM IOT node.
  */

#include "CodalConfig.h"
#include "STM32IotNode.h"
#include "STM32IotNodeGyroscope.h"

namespace codal
{
  /**
    * Constructor.
    *
    * Create a representation of the accelerometer on the STM32 IOT node
    *
    */
  STM32IotNodeGyroscope::STM32IotNodeGyroscope( CoordinateSpace& coordinateSpace )
  : Gyroscope( coordinateSpace ),
    gyroscopeDrv(&Lsm6dslGyroDrv),
    isInitialized(false),
    previousSampleTime(0)
  {
    previousSampleTime = system_timer_current_time();
    configure();
    // Configure for a 20 Hz update frequency by default.
    if(EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(this->id, SENSOR_UPDATE_NEEDED, this, &STM32IotNodeGyroscope::onSampleEvent, MESSAGE_BUS_LISTENER_IMMEDIATE);

    // Ensure we're scheduled to don't update the data periodically
    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;
    status &= ~DEVICE_COMPONENT_STATUS_SYSTEM_TICK;
    // Indicate that we're up and running.
    status |= DEVICE_COMPONENT_RUNNING;
  }

/*
 * Event Handler for periodic sample timer
 */
void STM32IotNodeGyroscope::onSampleEvent(Event)
{
    requestUpdate();
}

  uint8_t STM32IotNodeGyroscope::getBestAdaptedODRValue(){
    if ( !samplePeriod )
      samplePeriod = 1;

    float frequency = 1000.0f / (float) samplePeriod;
    uint8_t odr = LSM6DSL_ODR_POWER_DOWN;
    if(frequency <= 20){
      odr = LSM6DSL_ODR_13Hz;
      frequency = 13.f;
    }
    else if(frequency <= 39.5){
      odr = LSM6DSL_ODR_26Hz;
      frequency = 26.f;
    }
    else if(frequency <= 78.5){
      odr = LSM6DSL_ODR_52Hz;
      frequency = 52.f;
    }
    else if(frequency <= 156.5){
      odr = LSM6DSL_ODR_104Hz;
      frequency = 104.f;
    }
    else if(frequency <= 312.5){
      odr = LSM6DSL_ODR_208Hz;
      frequency = 208.f;
    }
    else if(frequency <= 625){
      odr = LSM6DSL_ODR_416Hz;
      frequency = 416.f;
    }
    else if(frequency > 625){
      odr = LSM6DSL_ODR_833Hz;
      frequency = 833.f;
    } else{
      odr = LSM6DSL_ODR_POWER_DOWN;
      frequency = 0;
    }
    samplePeriod = 1000.0f / frequency;
    return odr;
  }

  uint8_t STM32IotNodeGyroscope::getBestAdaptedFSValue(){
    if ( !sampleRange )
      sampleRange = 1;

    uint8_t fs = 0;
    if(sampleRange*100 <= 4){
      fs = LSM6DSL_GYRO_FS_245;
      sampleRange = 2;
    }
    else if(sampleRange <= 8){
      fs = LSM6DSL_GYRO_FS_500;
      sampleRange = 5;
    }
    else if(sampleRange <= 16 ){
      fs = LSM6DSL_GYRO_FS_1000;
      sampleRange = 10;
    } else {
      fs = LSM6DSL_GYRO_FS_2000;
      sampleRange = 20;
    }
    return fs;
  }

  /**
  * Configures the accelerometer for G range and sample rate defined
  * in this object. The nearest values are chosen to those defined
  * that are supported by the hardware. The instance variables are then
  * updated to reflect reality.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the accelerometer could not be configured.
  *
  */
  int STM32IotNodeGyroscope::configure( )
  {
    int ret = DEVICE_OK;
    uint16_t ctrl = 0x0000;
    GYRO_InitTypeDef LSM6DSL_InitStructure;

    if(gyroscopeDrv->ReadID() != LSM6DSL_ACC_GYRO_WHO_AM_I)
    {
      return DEVICE_I2C_ERROR;
    }

    /* Configure Mems : data rate, power mode, full scale and axes */
    LSM6DSL_InitStructure.Power_Mode = 0;
    LSM6DSL_InitStructure.Output_DataRate = getBestAdaptedODRValue();
    LSM6DSL_InitStructure.Axes_Enable = 0;
    LSM6DSL_InitStructure.Band_Width = 0;
    LSM6DSL_InitStructure.BlockData_Update = LSM6DSL_BDU_BLOCK_UPDATE;
    LSM6DSL_InitStructure.Endianness = 0;
    LSM6DSL_InitStructure.Full_Scale = getBestAdaptedFSValue(); 

    /* Configure MEMS: data rate, full scale  */
    ctrl = (LSM6DSL_InitStructure.Full_Scale | LSM6DSL_InitStructure.Output_DataRate);

    /* Configure MEMS: BDU and Auto-increment for multi read/write */
    ctrl |= ((LSM6DSL_InitStructure.BlockData_Update | LSM6DSL_ACC_GYRO_IF_INC_ENABLED) << 8);

    /* Initialize component */
    gyroscopeDrv->Init(ctrl);

    return ret;
  }

  /**
  * Poll to see if new data is available from the hardware. If so, update it.
  * n.b. it is not necessary to explicitly call this function to update data
  * (it normally happens in the background when the scheduler is idle), but a check is performed
  * if the user explicitly requests up to date data.
  *
  * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
  *
  */
   int STM32IotNodeGyroscope::requestUpdate()
  {
    CODAL_TIMESTAMP actualTime = system_timer_current_time();
    CODAL_TIMESTAMP delta = actualTime - previousSampleTime;
    if(delta > samplePeriod || previousSampleTime > actualTime){
      return updateSample();
    }
    return DEVICE_OK;
  }

  int STM32IotNodeGyroscope::updateSample()
  {
    if ( !isInitialized )
      configure();
    float pfData[3];
    
    if(gyroscopeDrv != NULL)
    {
      if(gyroscopeDrv->GetXYZ!= NULL)
      {
        gyroscopeDrv->GetXYZ(pfData);
        sample.x = pfData[0] / 100;
        sample.y = pfData[1] / 100;
        sample.z = pfData[2] / 100;
        previousSampleTime = system_timer_current_time();
        update(sample);
        return DEVICE_OK;
      }
    }
    return DEVICE_I2C_ERROR;
  }

}
