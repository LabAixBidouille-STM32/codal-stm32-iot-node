/**
  * Class definition for accelerometer.
  * Represents the accelerometer on the STM IOT node.
  */

#include "CodalConfig.h"
#include "STM32IotNode.h"
#include "STM32IotNodeAccelerometer.h"

namespace codal
{

  /**
    * Constructor.
    *
    * Create a representation of the accelerometer on the STM32 IOT node
    *
    */
  STM32IotNodeAccelerometer::STM32IotNodeAccelerometer(CoordinateSpace& coordinateSpace )
  : Accelerometer( coordinateSpace ), 
    accelerometerDrv(&Lsm6dslAccDrv),
    isInitialized(false)
  {
  }

  uint8_t STM32IotNodeAccelerometer::getBestAdaptedODRValue(){
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

  uint8_t STM32IotNodeAccelerometer::getBestAdaptedFSValue(){
    if ( !sampleRange )
      sampleRange = 1;

    uint8_t fs = 0;
    if(sampleRange <= 2){
      fs = LSM6DSL_ACC_FULLSCALE_2G;
      sampleRange = 2;
    }
    else if(sampleRange <= 6){
      fs = LSM6DSL_ACC_FULLSCALE_4G;
      sampleRange = 4;
    }
    else if(sampleRange <= 12 ){
      fs = LSM6DSL_ACC_FULLSCALE_8G;
      sampleRange = 8;
    } else {
      fs = LSM6DSL_ACC_FULLSCALE_16G;
      sampleRange = 16;
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
  int STM32IotNodeAccelerometer::configure( )
  {
    int ret = DEVICE_OK;
    uint16_t ctrl = 0x0000;
    ACCELERO_InitTypeDef LSM6DSL_InitStructure;

    if(accelerometerDrv->ReadID() != LSM6DSL_ACC_GYRO_WHO_AM_I)
    {
      return DEVICE_I2C_ERROR;
    }

    /* MEMS configuration : Fill the ACCELERO accelerometer structure */
    LSM6DSL_InitStructure.AccOutput_DataRate = getBestAdaptedODRValue();
    LSM6DSL_InitStructure.Axes_Enable = 0;
    LSM6DSL_InitStructure.AccFull_Scale = getBestAdaptedFSValue();
    LSM6DSL_InitStructure.BlockData_Update = LSM6DSL_BDU_BLOCK_UPDATE;
    LSM6DSL_InitStructure.High_Resolution = 0;
    LSM6DSL_InitStructure.Communication_Mode = 0;
        
    /* Configure MEMS: data rate, full scale  */
    ctrl =  (LSM6DSL_InitStructure.AccOutput_DataRate | LSM6DSL_InitStructure.AccFull_Scale);
    
    /* Configure MEMS: BDU and Auto-increment for multi read/write */
    ctrl |= ((LSM6DSL_InitStructure.BlockData_Update | LSM6DSL_ACC_GYRO_IF_INC_ENABLED) << 8);

    /* Configure the ACCELERO accelerometer main parameters */
    accelerometerDrv->Init(ctrl);
    isInitialized = true;
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
  int STM32IotNodeAccelerometer::requestUpdate()
  {
    printf("STM32IotNodeAccelerometer::requestUpdate()\n");
    if ( !isInitialized )
      configure();
    int16_t piData[3];

    if(accelerometerDrv != NULL)
    {
      if(accelerometerDrv->GetXYZ!= NULL)
      {
        accelerometerDrv->GetXYZ(piData);
        sample.x = piData[0] / 100;
        sample.y = piData[1] / 100;
        sample.z = piData[2] / 100;
        return DEVICE_OK;
      }
    }
    return DEVICE_I2C_ERROR;
  }

}
