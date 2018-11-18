#include "SerialSample.h"
#include "stm32l4xxI2C.h"

// Sensors drivers present in the BSP library
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"

void Serial_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("* Demonstration de la communication serie *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);
    float sensor_value = 0;
    
    BSP_TSENSOR_Init();

    while(1) {
        iotNode.io.led.setDigitalValue(1);

        sensor_value = BSP_TSENSOR_ReadTemp();
        int isensor_value  = (int) sensor_value;
        int dsensor_value = int((sensor_value - isensor_value)*100.);
        printf("\nTEMPERATURE = %d.%d degC\n", isensor_value, dsensor_value);

        iotNode.io.led.setDigitalValue(0);
        iotNode.sleep(1000);
    }
}