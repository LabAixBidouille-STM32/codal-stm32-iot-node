#include "CodalConfig.h"

#include "HTS221.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        float sensor_value = default_device_instance->temperature.getValue();
        int isensor_value  = (int) sensor_value/10.;
        int dsensor_value = int((sensor_value/10. - isensor_value)*10.);
        printf("\nTEMPERATURE = %d.%d degC\n", isensor_value, dsensor_value);
}

void HTS221_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("* Demonstration du capteur de temperature *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_THERMOMETER, SENSOR_UPDATE_NEEDED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}