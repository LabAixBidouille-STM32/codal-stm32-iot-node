#include "CodalConfig.h"

#include "HTS221_HUMIDITY.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        float sensor_value = default_device_instance->humidity.getValue();
        int isensor_value  = (int) sensor_value/10.;
        int dsensor_value = int((sensor_value/10. - isensor_value)*10.);
        printf("\nHUMIDITY = %d.%d %%\n", isensor_value, dsensor_value);
}

void HTS221_HUMIDITY_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*   Demonstration du capteur d'humidite   *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_HUMIDITY, SENSOR_UPDATE_NEEDED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}