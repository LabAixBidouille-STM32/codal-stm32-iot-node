/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "BLE.h"
#include "services/HealthThermometerService.h"
#include "BLE_Thermometer.h"

#include "stm32l475e_iot01_tsensor.h"

using namespace codal;

const static char     DEVICE_NAME[]        = "Therm";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEALTH_THERMOMETER_SERVICE};

static float currentTemperature   = 39.6;
static HealthThermometerService *thermometerServicePtr;

/* Restart Advertising on disconnection*/
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *)
{
    BLE::Instance().gap().startAdvertising();
}

void updateSensorValue(void) {
    printf("updateSensorValue()\n");
    currentTemperature = BSP_TSENSOR_ReadTemp();

    printf("currentTemperature = %d\n", (int)currentTemperature);
    thermometerServicePtr->updateTemperature(currentTemperature);
}

void periodicCallback(Event e)
{
    default_device_instance->io.led.setDigitalValue(!default_device_instance->io.led.getDigitalValue()); /* Do blinky on LED1 while we're waiting for BLE events */

    if (BLE::Instance().gap().getState().connected) {
        updateSensorValue();
    }
}

void onBleInitError(BLE &ble, ble_error_t error)
{
    printf("Init Error : %d \n", error);
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    printf("bleInitComplete()\n");

    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        onBleInitError(ble, error);
        return;
    }

    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup primary service. */
    thermometerServicePtr = new HealthThermometerService(ble, currentTemperature, HealthThermometerService::LOCATION_EAR);

    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::THERMOMETER_EAR);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    ble.gap().startAdvertising();
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    printf("scheduleBleEventsProcessing()\n");
    BLE &ble = BLE::Instance();
    ble.processEvents();
}

void BLE_Thermometer_main(codal::STM32IotNode& iotNode)
{
    printf("\n");
    printf("*******************************************\n");
    printf("*  Demonstration de la communication BLE  *\n");
    printf("*******************************************\n");

    iotNode.sleep(1000);
    
    BSP_TSENSOR_Init();

    system_timer_event_every(500, DEVICE_ID_BLE, SENSOR_UPDATE_NEEDED);
    EventModel::defaultEventBus->listen(DEVICE_ID_BLE, SENSOR_UPDATE_NEEDED, periodicCallback, MESSAGE_BUS_LISTENER_IMMEDIATE);
    
    
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);
}
