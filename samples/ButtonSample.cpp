#include "CodalConfig.h"
#include "ButtonSample.h"

using namespace codal;

void onButton(Event e)
{
    if (e.source == DEVICE_ID_BUTTON_A)
        printf("BUTTON USER: ");

    if (e.value == DEVICE_BUTTON_EVT_DOWN)
        printf("DOWN");

    if (e.value == DEVICE_BUTTON_EVT_UP)
        printf("UP");

    if (e.value == DEVICE_BUTTON_EVT_CLICK)
        printf("CLICK");

    if (e.value == DEVICE_BUTTON_EVT_LONG_CLICK)
        printf("LONG_CLICK");

    if (e.value == DEVICE_BUTTON_EVT_HOLD)
        printf("HOLD");

    if (e.value == DEVICE_BUTTON_EVT_DOUBLE_CLICK)
        printf("DOUBLE_CLICK");
    printf("\n");
}


void ButtonSample_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*       Demonstration du bouton USER      *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    EventModel::defaultEventBus->listen(DEVICE_ID_BUTTON_A, DEVICE_EVT_ANY, onButton);
    
    iotNode.sleep(1000);
    while(1) {
        iotNode.sleep(5000);
        printf("BUTTONA: %d clicks\n", iotNode.buttonUSER.wasPressed());
    }
}