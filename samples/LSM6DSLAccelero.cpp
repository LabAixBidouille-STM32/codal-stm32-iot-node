#include "CodalConfig.h"

#include "LSM6DSLAccelero.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
        float sensor_value = default_device_instance->accelerometer.getX();
        int isensor_value  = (int) sensor_value;
        int dsensor_value = int((sensor_value - isensor_value)*10.);
        printf("ACCELERATION X = %d.%d \n", isensor_value, dsensor_value);
}

void LSM6DSLAccelero_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*     Demonstration de l'accelerometre    *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_ACCELEROMETER, SENSOR_UPDATE_NEEDED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}