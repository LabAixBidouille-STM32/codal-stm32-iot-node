#include "ble_debug.h"
#include "ble_gp_timer.h"
#include "ble_hci.h"
#include "ble_sm.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_gap.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_utils.h"
#include "common.h"
#include "hci_const.h"
#include "hci_le.h"
#include "hw.h"
#include "osal.h"
#include "stm32_bluenrg_ble.h"
#include "stm32l4xx_ll_rcc.h"
#include "tl_ble_io.h"
#include "tl_ble_reassembly.h"

#include "Joystick.h"

using namespace codal;

void attach_HCI_CB(void (*callback)(void* pckt));

void (*HCI_callback)(void*);

/**
 * @brief  This function allows to attach a HCI callback from a profil.
 * @param  callback: function pointer of the callback to attach.
 * @retval None
 */
void attach_HCI_CB(void (*callback)(void* pckt)) {
    HCI_callback = callback;
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void* pckt) {
    HCI_callback(pckt);
}

const char* name        = "24hBTLE";
uint8_t SERVER_BDADDR[] = {0x16, 0x34, 0x00, 0xE1, 0x80, 0x02};

// AxesRaw_t axes_data;
uint32_t previousSecond = 0;

#define BDADDR_SIZE 6

typedef enum { CSTS_DISCONNECTED, CSTS_CONNECTED, CSTS_DISCONNECTED_BY_USER } Connection_status_t;

/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, \
                      uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0)    \
    do {                                                                                         \
        uuid_struct.uuid128[0]  = uuid_0;                                                        \
        uuid_struct.uuid128[1]  = uuid_1;                                                        \
        uuid_struct.uuid128[2]  = uuid_2;                                                        \
        uuid_struct.uuid128[3]  = uuid_3;                                                        \
        uuid_struct.uuid128[4]  = uuid_4;                                                        \
        uuid_struct.uuid128[5]  = uuid_5;                                                        \
        uuid_struct.uuid128[6]  = uuid_6;                                                        \
        uuid_struct.uuid128[7]  = uuid_7;                                                        \
        uuid_struct.uuid128[8]  = uuid_8;                                                        \
        uuid_struct.uuid128[9]  = uuid_9;                                                        \
        uuid_struct.uuid128[10] = uuid_10;                                                       \
        uuid_struct.uuid128[11] = uuid_11;                                                       \
        uuid_struct.uuid128[12] = uuid_12;                                                       \
        uuid_struct.uuid128[13] = uuid_13;                                                       \
        uuid_struct.uuid128[14] = uuid_14;                                                       \
        uuid_struct.uuid128[15] = uuid_15;                                                       \
    } while (0)

class JoystickServiceClass : CodalComponent {
  public:
    JoystickServiceClass(void) : CodalComponent(DEVICE_ID_JOYSTICK, 0) {
        // Ensure we're scheduled to don't update the data periodically
        status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;
        status &= ~DEVICE_COMPONENT_STATUS_SYSTEM_TICK;
        // Indicate that we're up and running.
        status |= DEVICE_COMPONENT_RUNNING;
    }

    virtual int init() { return DEVICE_OK; }

    /**
     * Implement this function to receive a callback every SCHEDULER_TICK_PERIOD_MS.
     */
    virtual void periodicCallback() {}

    /**
     * Implement this function to receive a callback when the device is idling.
     */
    virtual void idleCallback() {}

    void setConnectable(void) {}

    int isConnected(void) { return true; }

    tBleStatus Add_Joystick(void) { return BLE_STATUS_SUCCESS; }

    void GAP_ConnectionComplete_CB(uint8_t addr[BDADDR_SIZE], uint16_t handle) {}

    void GAP_DisconnectionComplete_CB(void) {}

    void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t* att_data) {}

    void Update_LED1_State() {}

    void Update_LED2_State() {}
};

void Joystick_main(codal::STM32IotNode& iotNode) {
    printf("\n");
    printf("*******************************************\n");
    printf("*    Demonstration du service Joystick    *\n");
    printf("*******************************************\n");

    JoystickServiceClass JoystickService;

    while (1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);
        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}
