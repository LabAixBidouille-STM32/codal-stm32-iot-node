/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : ble_hci.h
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 19-July-2012
* Description        : Constants and functions for HCI layer. See Bluetooth Core
*                      v 4.0, Vol. 2, Part E.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __BLE_HCI_H_
#define __BLE_HCI_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "link_layer.h"
#include "bluenrg_private_hal_types.h"
#include "list.h"

#define HCI_READ_PACKET_SIZE                    128 //71

/**
 * Maximum payload of HCI commands that can be sent. Change this value if needed.
 * This value can be up to 255.
 */
//#define HCI_MAX_PAYLOAD_SIZE 128

/*** Data types ***/

/* structure used to read received data */
typedef struct _tHciDataPacket
{
  tListNode currentNode;
  uint8_t dataBuff[HCI_READ_PACKET_SIZE];
  uint8_t data_len;
} tHciDataPacket;

typedef enum
{
  BUSY,
  AVAILABLE
} HCI_CMD_STATUS_t;


/**
 * This function must be used to pass the packet received from the HCI
 * interface to the BLE Stack HCI state machine.
 *
 * @param[in] hciReadPacket    The packet that is received from HCI interface.
 *
 */
void HCI_Input(tHciDataPacket *hciReadPacket);

/**
 * Initialization function. Must be done before any data can be received from
 * BLE controller.
 */
void HCI_Init(void);

/**
 * Callback used to pass events to application.
 *
 * @param[in] pckt    The event.
 *
 */
extern void HCI_Event_CB(void *pckt);

/**
 * Processing function that must be called after an event is received from
 * HCI interface. Must be called outside ISR. It will call HCI_Event_CB if
 * necessary.
*/
void HCI_Process(void);

/**
 * @brief Check if queue of HCI event is empty or not.
 * @note This funtion can be used to check if the event queue from BlueNRG is empty. This
 *        is useful when checking if it is safe to go to sleep. 
 * @return TRUE if event queue is empty. FALSE otherwhise.
 */
BOOL HCI_Queue_Empty(void);
/**
 * Iterrupt service routine that must be called when the BlueNRG 
 * reports a packet received or an event to the host through the 
 * BlueNRG interrupt line.
 */
#ifdef __DMA_LP__
void HCI_Isr(uint8_t *buffer, uint8_t event_payload_len);
void HCI_Process_Notification_Request(void);
void HCI_Cmd_Status(HCI_CMD_STATUS_t Hci_Cmd_Status);
void HCI_Wait_For_Response(void);
#else
void HCI_Isr(void);
void HCI_HandleSPI(void);
#endif /* __DMA_LP__ */

extern tListNode hciReadPktPool;
extern tListNode hciReadPktRxQueue;

#ifdef __cplusplus
}
#endif


#endif /* __BLE_HCI_H_ */
