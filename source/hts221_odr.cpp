#include "hts221.h"
#include "hts221_odr.h"
#include "stm32.h"

using namespace codal;

  /**
  * Configures output data rate register
  *
  *
  */
  void updateODR(uint8_t odr){
    uint8_t tmp;
    /* Read CTRL_REG1 */
    tmp = SENSOR_IO_Read(HTS221_I2C_ADDRESS, HTS221_CTRL_REG1);
    
    /* Set default ODR */
    tmp &= ~HTS221_ODR_MASK;
    tmp |= odr & HTS221_ODR_MASK; /*Set ODR*/
    
    /* Apply settings to CTRL_REG1 */
    SENSOR_IO_Write(HTS221_I2C_ADDRESS, HTS221_CTRL_REG1, tmp);
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