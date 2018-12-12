#include "CodalConfig.h"
#include "GPIO.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void onSampleEvent(Event e){
    int value = default_device_instance->io.A0.getAnalogValue();
    default_device_instance->io.D6.setAnalogValue(value);
    printf("A0 = %d\n", value);
}


void GPIO_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*           Demonstration de I/O          *\n");
    printf("*******************************************\n");
    //iotNode.io.A1.setAnalogValue(512);
    iotNode.sleep(1000);
    system_timer_event_every(1000, ID_PIN_P0, SENSOR_UPDATED);
    EventModel::defaultEventBus->listen(ID_PIN_P0, SENSOR_UPDATED, onSampleEvent);

    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);

        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}