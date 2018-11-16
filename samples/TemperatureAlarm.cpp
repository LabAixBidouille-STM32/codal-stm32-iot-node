#include "STM32IotNode.h"
#include "stm32.h"
#include "CodalDmesg.h"
#include "bluetooth/services/TemperatureService.h"

/*
 * Temperature Alarm
 * Uses the Bluetooth Event Service to inform registered clients whenever the temperature falls below xx or rises above yy
 */
using namespace codal;

STM32IotNode* piotNode;

uint16_t state      = 0; // 0=ok, 1=alarming because it's cold, 2=alarming because it's hot
uint16_t ok         = 0;
uint16_t any        = 0;
uint16_t temp_alarm = 9000;
uint16_t set_lower  = 9001;
uint16_t set_upper  = 9002;
uint16_t cold       = 1;
uint16_t hot        = 2;
uint8_t  upper      = 24;
uint8_t  lower      = 19;
int reading_count   = 0;

void onSetLower(Event e)
{
    lower = e.value;
    DMESG("L=", lower);
}

void onSetUpper(Event e)
{
    upper = e.value;
    DMESG("U=", upper);
}

void onTemperatureReading(Event e)
{
    int temperature = piotNode->temperature.getValue();
    reading_count++;
    if (reading_count == 10) { 
        DMESG("Temp=",temperature);
        reading_count = 0;
    }

    if (temperature > upper && state == ok) {
        Event evt(temp_alarm, hot);
        state = hot;
        return;
    }

    if (temperature < lower && state == ok) {
        Event evt(temp_alarm, cold);
        state = cold;
        return;
    }

    if (temperature >= lower && temperature <= upper && state != ok) {
        Event evt(temp_alarm, ok);
        state = ok;
    }  
}

void TemperatureAlarm_main(STM32IotNode& _iotNode)
{
    piotNode = &_iotNode;
    DMESG("TEMP ALARM");
    //new TemperatureService(piotNode->ble, piotNode->temperature);
    // listen for client events which set the upper and lower temperature limits
    piotNode->messageBus.listen(set_lower, any, onSetLower);
    piotNode->messageBus.listen(set_upper, any, onSetUpper);
    // listen to the temperature sensor
    piotNode->messageBus.listen(DEVICE_ID_THERMOMETER, SENSOR_UPDATE_NEEDED, onTemperatureReading);
}
