#include "CodalConfig.h"
#include "GPIO.h"
#include "stm32l4xxI2C.h"

using namespace codal;

void GPIO_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*           Demonstration de I/O          *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    int state = 0;
    while(1) {
        iotNode.io.D6.setDigitalValue(1);
        iotNode.sleep(100);
        iotNode.io.D6.setDigitalValue(0);
        iotNode.sleep(100);
        iotNode.io.D5.setAnalogValue(state);
        state =(state + 10) % 1024; 
    }
}