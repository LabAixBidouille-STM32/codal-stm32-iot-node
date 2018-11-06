#include "Blink.h"

void Blink_main(codal::STM32IotNode& iotNode){
	int state = 0;
	while(1)
    {    
		iotNode.io.led.setDigitalValue(state);
        iotNode.io.led2.setDigitalValue(!state);
        iotNode.sleep(1000);
        state = !state;
    }
}