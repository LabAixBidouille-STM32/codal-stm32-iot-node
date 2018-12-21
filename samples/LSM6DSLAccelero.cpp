#include "CodalConfig.h"

#include "LSM6DSLAccelero.h"
#include "stm32l4xxI2C.h"

using namespace codal;

const char* gestureName(int value){
    switch(value){
        case ACCELEROMETER_EVT_NONE      :
            return "NONE";
        case ACCELEROMETER_EVT_TILT_UP   :
            return "TILT_UP";
        case ACCELEROMETER_EVT_TILT_DOWN :
            return "TILT_DOWN";
        case ACCELEROMETER_EVT_TILT_LEFT :
            return "TILT_LEFT";
        case ACCELEROMETER_EVT_TILT_RIGHT:
            return "TILT_RIGHT";
        case ACCELEROMETER_EVT_FACE_UP   :
            return "FACE_UP";
        case ACCELEROMETER_EVT_FACE_DOWN :
            return "FACE_DOWN";
        case ACCELEROMETER_EVT_FREEFALL  :
            return "FREEFALL";
        case ACCELEROMETER_EVT_3G        :
            return "3G";
        case ACCELEROMETER_EVT_6G        :
            return "6G";
        case ACCELEROMETER_EVT_8G        :
            return "8G";
        case ACCELEROMETER_EVT_SHAKE     :
            return "SHAKE";
        default :
            return "";

    }

}

void onSampleEvent(Event e){
    Sample3D sample = default_device_instance->accelerometer.getSample();
    if(e.source == DEVICE_ID_ACCELEROMETER)
        printf("(%ld) ACCELERATION  = |X : %d, Y : %d, Z : %d|\n", e.timestamp,sample.x, sample.y, sample.z);
    if(e.source == DEVICE_ID_GESTURE)
        printf("(%ld) GESTURE  = %s\n", e.timestamp, gestureName(e.value));        
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