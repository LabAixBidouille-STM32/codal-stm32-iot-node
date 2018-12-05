#include "CodalConfig.h"

#include "VL53L0X.h"
#include "i2c.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
    if(default_device_instance != nullptr){
        uint32_t distance = default_device_instance->distance.getValue();
        printf("DISTANCE = %ld mm\n", distance);
    }
}

void VL53L0X_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*     Demonstration du capteur de ToF     *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_DISTANCE, SENSOR_UPDATED, onSampleEvent);

    iotNode.distance.getValue();

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);        
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}