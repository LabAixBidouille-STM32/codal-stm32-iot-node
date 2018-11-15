/**
  * Class definition for temperature.
  * Represents the temperature on the STM IOT node.
  */

#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "STM32IotNode.h"
#include "STM32IotNodeDistance.h"

namespace codal
{

/**
  * Constructor.
  *
  * Create a representation of the temperature on the STM32 IOT node
  *
  */
STM32IotNodeDistance::STM32IotNodeDistance( STM32L4xxI2C& i2c )
: Sensor(DEVICE_ID_DISTANCE)
, _i2c( i2c )
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
 return DEVICE_OK;
}


/**
  * Implement this function to receive a function call after the devices'
  * device model has been instantiated.
  */
int STM32IotNodeDistance::init()
{
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

int STM32IotNodeDistance::readValue()
{
    return 5;
}

}
