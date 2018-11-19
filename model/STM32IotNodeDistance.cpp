/**
  * Class definition for temperature.
  * Represents the temperature on the STM IOT node.
  */

#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "STM32IotNode.h"
#include "STM32IotNodeDistance.h"

using namespace codal;

/**
  * Constructor.
  *
  * Create a representation of the temperature on the STM32 IOT node
  *
  */
STM32IotNodeDistance::STM32IotNodeDistance()
: Sensor(DEVICE_ID_DISTANCE),
  sensor_vl53l0x(default_i2c_sensors_bus, nullptr, nullptr),
  needInit(true)
{

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

int STM32IotNodeDistance::configure()
{
   // Switch off VL53L0X component.
  sensor_vl53l0x.VL53L0X_Off();

  // Initialize VL53L0X top component.
  status = sensor_vl53l0x.InitSensor(0x10);
  if(status)
  {
    printf("Init sensor_vl53l0x failed...");
    return DEVICE_I2C_ERROR;
  }
  printf("Init sensor_vl53l0x ok...");

  return DEVICE_OK;
}


/**
  * Implement this function to receive a function call after the devices'
  * device model has been instantiated.
  */
int STM32IotNodeDistance::init()
{
    if(!needInit)
      return DEVICE_OK;

    sensor_vl53l0x.setGpio0(&default_device_instance->io.pc6);
    sensor_vl53l0x.setGpio1Int(&default_device_instance->io.pc7);
    
    needInit = false;
    return configure();
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

int STM32IotNodeDistance::readValue()
{
  init();
  // Read Range.
  uint32_t distance;
  int status = sensor_vl53l0x.GetDistance(&distance);

  printf("| Distance [mm]: %ld |  \n", distance);
  
  if (status != VL53L0X_ERROR_NONE){
    printf("VL53L0X_ERROR : %d\n", status );
    return DEVICE_I2C_ERROR;
  }
  
  return distance;
}
