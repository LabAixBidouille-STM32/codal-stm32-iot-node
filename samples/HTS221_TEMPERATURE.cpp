#include "CodalConfig.h"

#include "HTS221_TEMPERATURE.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        int sensor_value = default_device_instance->temperature.getValue();
        printf("\nTEMPERATURE = %d degC\n", sensor_value);
}

void HTS221_TEMPERATURE_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("* Demonstration du capteur de temperature *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_THERMOMETER, SENSOR_UPDATED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}