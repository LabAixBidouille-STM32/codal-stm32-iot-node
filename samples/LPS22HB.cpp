#include "CodalConfig.h"

#include "LPS22HB.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        float sensor_value = default_device_instance->pressure.getValue();
        int isensor_value  = (int) sensor_value;
        int dsensor_value = int((sensor_value - isensor_value)*10.);
        printf("PRESSURE = %d.%d mBar\n", isensor_value, dsensor_value);
}

void LPS22HB_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*   Demonstration du capteur de pression  *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_PRESSURE, SENSOR_UPDATE_NEEDED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}