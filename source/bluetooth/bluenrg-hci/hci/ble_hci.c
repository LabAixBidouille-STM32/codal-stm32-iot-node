/**
  ******************************************************************************
  * @file    ble_hci.c 
  * @author  AMS/HESA Application Team
  * @brief   Function for managing HCI interface.
  ******************************************************************************
  *
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 

#include "osal.h"
#include "list.h"
#include "ble_status.h"
#include "tl_ble_io.h"
#include "tl_ble_hci.h"
#include "ble_hci.h"
#include "ble_gp_timer.h"
#include "ble_debug.h"
#include "hci_const.h"

#include "stm32_bluenrg_ble.h"

#define DEFAULT_TIMEOUT (33000)

#if BLE_CONFIG_DBG_ENABLE
#undef PRINTF
#endif

#define HCI_LOG_ON 0

#define HCI_READ_PACKET_NUM_MAX 		 (0x40)

tListNode hciReadPktPool;
tListNode hciReadPktRxQueue;

// betzw - DEBUG:
//#define POOL_CNT
#ifdef POOL_CNT
#include <stdio.h>
static unsigned int nr_hciReadPktPool;
static unsigned int lowest_nr_hciReadPktPool;
#endif // POOL_CNT

/* pool of hci read packets */
static tHciDataPacket   hciReadPacketBuffer[HCI_READ_PACKET_NUM_MAX];

static volatile uint8_t hci_timer_id;
static volatile uint8_t hci_timeout;

void hci_timeout_callback(void)
{
  hci_timeout = 1;
  return;
}

void HCI_Init(void)
{
  uint8_t index;
  
#ifdef POOL_CNT
  nr_hciReadPktPool = 0;
#endif // POOL_CNT

  /* Initialize list heads of ready and free hci data packet queues */
  list_init_head (&hciReadPktPool);
  list_init_head (&hciReadPktRxQueue);
  
  /* Initialize the queue of free hci data packets */
  for (index = 0; index < HCI_READ_PACKET_NUM_MAX; index++)
  {
    list_insert_tail(&hciReadPktPool, (tListNode *)&hciReadPacketBuffer[index]);
#ifdef POOL_CNT
    nr_hciReadPktPool++;
#endif // POOL_CNT
  }

#ifdef POOL_CNT
  lowest_nr_hciReadPktPool = nr_hciReadPktPool;
#endif // POOL_CNT
}

#define HCI_PCK_TYPE_OFFSET                 0
#define  EVENT_PARAMETER_TOT_LEN_OFFSET     2

/**
 * Verify if HCI packet is correctly formatted.
 *
 * @param[in] hciReadPacket    The packet that is received from HCI interface.
 * @return 0 if HCI packet is as expected
 */
int HCI_verify(const tHciDataPacket * hciReadPacket)
{
  const uint8_t *hci_pckt = hciReadPacket->dataBuff;
  
  if(hci_pckt[HCI_PCK_TYPE_OFFSET] != HCI_EVENT_PKT)
    return 1;  /* Incorrect type. */
  
  if(hci_pckt[EVENT_PARAMETER_TOT_LEN_OFFSET] != hciReadPacket->data_len - (1+HCI_EVENT_HDR_SIZE))
    return 2; /* Wrong length (packet truncated or too long). */
  
  return 0;      
}

void HCI_Process(void)
{
  tHciDataPacket * hciReadPacket = NULL;
  
#ifdef POOL_CNT
  printf("betzw(%s, %d): nr_hciReadPktPool = %u (lowest = %u)\r\n", __func__, __LINE__, 
	 nr_hciReadPktPool, lowest_nr_hciReadPktPool);
#endif // POOL_CNT

  Disable_SPI_IRQ();
  uint8_t list_empty = list_is_empty(&hciReadPktRxQueue);        
  /* process any pending events read */
  while(list_empty == FALSE)
  {
    list_remove_head (&hciReadPktRxQueue, (tListNode **)&hciReadPacket);
    Enable_SPI_IRQ();
    HCI_Event_CB(hciReadPacket->dataBuff);
    Disable_SPI_IRQ();
    list_insert_tail(&hciReadPktPool, (tListNode *)hciReadPacket);
#ifdef POOL_CNT
    nr_hciReadPktPool++;
#endif
    list_empty = list_is_empty(&hciReadPktRxQueue);
  }
  /* Explicit call to HCI_HandleSPI(), since it cannot be triggered by ISR if IRQ is kept high by
  BlueNRG. */
  HCI_HandleSPI();
  Enable_SPI_IRQ();    
}

BOOL HCI_Queue_Empty(void)
{
  return list_is_empty(&hciReadPktRxQueue);
}

/**
 * When an interrupt is raised by BlueNRG,
 * just signal that a new event (availability of SPI data to be read)
 * needs to be processed.
 */
void HCI_Isr(void)
{
  signalEventsToProcess();
}

/**
 * Now, SPI Data are handled in user space.
 * In case it has to be called in ISR, take care to
 * call Disable_SPI_IRQ/Enable_SPI_IRQ in a proper way.
 * The calls Disable_SPI_IRQ/Enable_SPI_IRQ have not been removed
 * from this code for backward compatibility.
 */
void HCI_HandleSPI(void)
{
  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  
  Clear_SPI_EXTI_Flag();
  while(BlueNRG_DataPresent()){
    if (list_is_empty (&hciReadPktPool) == FALSE){
      
      /* enqueueing a packet for read */
      list_remove_head (&hciReadPktPool, (tListNode **)&hciReadPacket);
#ifdef POOL_CNT
      nr_hciReadPktPool--;
      if(nr_hciReadPktPool < lowest_nr_hciReadPktPool)
	      lowest_nr_hciReadPktPool = nr_hciReadPktPool;
#endif

      data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_READ_PACKET_SIZE);
      if(data_len > 0){                    
        hciReadPacket->data_len = data_len;
        if(HCI_verify(hciReadPacket) == 0) {
          list_insert_tail(&hciReadPktRxQueue, (tListNode *)hciReadPacket);
        } else {
          list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);
#ifdef POOL_CNT
	  nr_hciReadPktPool++;
#endif
	}
      }
      else {
        // Insert the packet back into the pool.
        list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);
#ifdef POOL_CNT
	nr_hciReadPktPool++;
#endif
      }
    }
    else{
      // HCI Read Packet Pool is empty, wait for a free packet.
      Clear_SPI_EXTI_Flag();
      return;
    }
    
    Clear_SPI_EXTI_Flag();
  }
}

void hci_write(const void* data1, const void* data2, uint8_t n_bytes1, uint8_t n_bytes2){
#if  HCI_LOG_ON
  PRINTF("HCI <- ");
  for(int i=0; i < n_bytes1; i++)
    PRINTF("%02X ", *((uint8_t*)data1 + i));
  for(int i=0; i < n_bytes2; i++)
    PRINTF("%02X ", *((uint8_t*)data2 + i));
  PRINTF("\n");    
#endif
  
  Hal_Write_Serial(data1, data2, n_bytes1, n_bytes2);
}

void hci_send_cmd(uint16_t ogf, uint16_t ocf, uint8_t plen, void *param)
{
  hci_command_hdr hc;
  
  hc.opcode = htobs(cmd_opcode_pack(ogf, ocf));
  hc.plen= plen;
  
  uint8_t header[HCI_HDR_SIZE + HCI_COMMAND_HDR_SIZE];
  header[0] = HCI_COMMAND_PKT;
  Osal_MemCpy(header+1, &hc, sizeof(hc));
  
  hci_write(header, param, sizeof(header), plen);
}
