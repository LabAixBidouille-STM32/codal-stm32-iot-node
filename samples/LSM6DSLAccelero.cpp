#include "CodalConfig.h"

#include "LSM6DSLAccelero.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
    default_device_instance->sleep(100);
    Sample3D sample = default_device_instance->accelerometer.getSample();
    printf("(%ld) ACCELERATION  = |X : %d, Y : %d, Z : %d|\n", system_timer_current_time(),sample.x, sample.y, sample.z);
        
}

void LSM6DSLAccelero_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*     Demonstration de l'accelerometre    *\n");
    printf("*******************************************\n");
    
    EventModel::defaultEventBus->listen(DEVICE_ID_ACCELEROMETER, DEVICE_EVT_ANY, onSampleEvent);
    EventModel::defaultEventBus->listen(DEVICE_ID_GESTURE, DEVICE_EVT_ANY, onSampleEvent);

    iotNode.sleep(500);

    iotNode.accelerometer.requestUpdate();
    
    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}