#include "CodalConfig.h"

#include "LSM6DSLAccelero.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
    default_device_instance->sleep(100);
    Sample3D sample = default_device_instance->gyroscope.getSample();
    printf("(%ld) ROTATION  = |rX : %d, rY : %d, rZ : %d|\n", system_timer_current_time(),sample.x, sample.y, sample.z);
        
}

void LSM6DSLGyro_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*     Demonstration du gyroscope    *\n");
    printf("*******************************************\n");
    
    EventModel::defaultEventBus->listen(DEVICE_ID_GYROSCOPE, DEVICE_EVT_ANY, onSampleEvent);

    iotNode.sleep(500);

    iotNode.gyroscope.requestUpdate();
    
    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}