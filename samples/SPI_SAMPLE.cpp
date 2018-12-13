#include "CodalConfig.h"
#include "SPI_SAMPLE.h"
#include "stm32l4xxSPI.h"



#define BLUENRG_PIN_SPI_MOSI   (codal::PinNumber::PC_12)
#define BLUENRG_PIN_SPI_MISO   (codal::PinNumber::PC_11)
#define BLUENRG_PIN_SPI_nCS    (codal::PinNumber::PD_13)
#define BLUENRG_PIN_SPI_RESET  (codal::PinNumber::PA_8)
#define BLUENRG_PIN_SPI_IRQ    (codal::PinNumber::PE_6)
#define BLUENRG_PIN_SPI_SCK    (codal::PinNumber::PC_10)

using namespace codal;

STM32L4xxPin PIN_BLE_MOSI (ID_PIN_BLE_MOSI, BLUENRG_PIN_SPI_MOSI,  codal::PIN_CAPABILITY_DIGITAL);
STM32L4xxPin PIN_BLE_MISO (ID_PIN_BLE_MISO, BLUENRG_PIN_SPI_MISO,  codal::PIN_CAPABILITY_DIGITAL);
STM32L4xxPin PIN_BLE_SCLK (ID_PIN_BLE_SCLK, BLUENRG_PIN_SPI_SCK,   codal::PIN_CAPABILITY_DIGITAL);
STM32L4xxPin PIN_BLE_CS   (ID_PIN_BLE_CS,   BLUENRG_PIN_SPI_nCS,   codal::PIN_CAPABILITY_DIGITAL);
STM32L4xxPin PIN_BLE_RST  (ID_PIN_BLE_RST,  BLUENRG_PIN_SPI_RESET, codal::PIN_CAPABILITY_DIGITAL);
STM32L4xxPin PIN_BLE_IRQ  (ID_PIN_BLE_IRQ,  BLUENRG_PIN_SPI_IRQ,   codal::PIN_CAPABILITY_DIGITAL);

void onSampleEvent(Event e){

}


void SPI_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*         Demonstration du bus SPI        *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);

    STM32L4xxSPI spi(PIN_BLE_MOSI, PIN_BLE_MISO, PIN_BLE_SCLK);

    spi.setMode(0, 8);
    spi.setFrequency(8000000);

    spi.init();

    PIN_BLE_CS.setDigitalValue(1);
    target_wait_us(500);
    
    PIN_BLE_CS.setDigitalValue(1);
    
    while(1) {
        iotNode.io.led.setDigitalValue(1);
        iotNode.sleep(1000);

        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}