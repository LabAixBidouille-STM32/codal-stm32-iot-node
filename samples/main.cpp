//#include "mbed.h"
//
//DigitalOut led1(LED1);
//
//// main() runs in its own thread in the OS
//int main() {
//    while (true) {
//        led1 = !led1;
//        wait(2);
//    }
//}

#include "STM32IotNode.h"

STM32IotNode IotNode;


void
onClick(Event)
{
 IotNode.serial.printf("CLICK\r\n");
 codal::Sample3D Sample = IotNode.gyroscope.getSample( );
 IotNode.serial.printf("   x: 0x%04X = %d\n", Sample.x, Sample.x );
 IotNode.serial.printf("   y: 0x%04X = %d\n", Sample.y, Sample.y );
 IotNode.serial.printf("   z: 0x%04X = %d\n", Sample.z, Sample.z );
}

int
main()
{
 IotNode.init();
 IotNode.serial.printf("*** STM32_IOT_NODE BLINKY TEST ***\r\n");
 IotNode.serial.printf( "sda: 0x%04X\r\n", IotNode.io.sda.name );
 IotNode.serial.printf( "scl: 0x%04X\r\n", IotNode.io.scl.name );

 IotNode.messageBus.listen(DEVICE_ID_BUTTON_A, DEVICE_BUTTON_EVT_CLICK, onClick);
 IotNode.i2c.setFrequency( 100000 );

 while(1)
 {
//  int16_t SamplePressure    = IotNode.pressure.getSample( );
//  IotNode.serial.printf("%d\n", SamplePressure );
  int16_t SampleHumidity    = IotNode.humidity.getSample( );
  int16_t SampleTemperature = IotNode.temperature.getSample( );
  IotNode.serial.printf("%d, %d\n", SampleHumidity, SampleTemperature );
//  codal::Sample3D Sample = IotNode.gyroscope.getSample( );
//  IotNode.serial.printf("%d, %d, %d\n", Sample.x, Sample.y, Sample.z );
//  IotNode.io.led.setDigitalValue(1);
//  IotNode.sleep(200);
//  IotNode.io.led.setDigitalValue(0);
  IotNode.sleep(200);
 }
}
