#include "stm32.h"
#include "STM32IotNode.h"

#if defined(BLE_TEMPERATURE_ALARM_SAMPLE)
    #include "TemperatureAlarm.h"
    #define SAMPLE_MAIN TemperatureAlarm_main
#elif defined(HTS221_TEMPERATURE_SAMPLE)
    #include "HTS221_TEMPERATURE.h"
    #define SAMPLE_MAIN HTS221_TEMPERATURE_main
#elif defined(HTS221_HUMIDITY_SAMPLE)
    #include "HTS221_HUMIDITY.h"
    #define SAMPLE_MAIN HTS221_HUMIDITY_main
#elif defined(LIS3MDL_SAMPLE)
    #include "LIS3MDL.h"
    #define SAMPLE_MAIN LIS3MDL_main
#elif defined(LSM6DSL_SAMPLE)
    #include "LSM6DSL.h"
    #define SAMPLE_MAIN LSM6DSL_main
#elif defined(LSM6DSL_GYRO_SAMPLE)
    #include "LSM6DSLGyro.h"
    #define SAMPLE_MAIN LSM6DSLGyro_main
#elif defined(LSM6DSL_ACCELERO_SAMPLE)
    #include "LSM6DSLAccelero.h"
    #define SAMPLE_MAIN LSM6DSLAccelero_main
#elif defined(LPS22HB_SAMPLE)
    #include "LPS22HB.h"
    #define SAMPLE_MAIN LPS22HB_main
#elif defined(VL53L0X_SAMPLE)
    #include "VL53L0X.h"
    #define SAMPLE_MAIN VL53L0X_main
#elif defined(BUTTON_SAMPLE)
    #include "Button.h"
    #define SAMPLE_MAIN BUTTON_main
#elif defined(GPIO_SAMPLE)
    #include "GPIO.h"
    #define SAMPLE_MAIN GPIO_main
#elif defined(ADC_SAMPLE)
    #include "ADC.h"
    #define SAMPLE_MAIN ADC_main
#elif defined(FLASH_MEMORY_SAMPLE)
    #include "FlashMemory.h"
    #define SAMPLE_MAIN FlashMemory_main
#elif defined(SERIAL_SAMPLE)
    #include "SerialSample.h"
    #define SAMPLE_MAIN Serial_main
#else
    #include "Blink.h"
    #define SAMPLE_MAIN Blink_main
#endif

using namespace codal;
STM32IotNode iotNode;

int main()
{
    iotNode.init();
    SAMPLE_MAIN(iotNode);
}