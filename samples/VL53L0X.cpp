#include "CodalConfig.h"

#include "VL53L0X.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        float sensor_value = default_device_instance->distance.readValue();
        int isensor_value  = (int) sensor_value;
        printf("DISTANCE = %d cm\n", isensor_value);
}

void VL53L0X_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*   Demonstration du capteur de ToF  *\n");
    printf("*******************************************\n");

    EventModel::defaultEventBus->listen(DEVICE_ID_DISTANCE, SENSOR_UPDATE_NEEDED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}