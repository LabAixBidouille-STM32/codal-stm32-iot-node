#include "LCDSample.h"
#include "stm32l4xxI2C.h"
#include "i2c.h"

using namespace codal;



uint8_t colorR = 255;
uint8_t colorG = 0;
uint8_t colorB = 0;

void LCD_main(codal::STM32IotNode& iotNode){
    printf("\n");
    printf("*******************************************\n");
    printf("*     Demonstration du LCD GROVE RGB      *\n");
    printf("*******************************************\n");
    iotNode.sleep(1000);
    // set up the LCD's number of columns and rows:
    rgb_lcd lcd(iotNode.i2c1, 16, 2);
    lcd.init();

    for(uint8_t i = 0; i < 4; i++){
        lcd.setColor(i);
        iotNode.sleep(1000);
    }

    lcd.setColorWhite();
    lcd.print("Hello world !");
    iotNode.sleep(1000);
    bool state = false;
    while(1) {
        iotNode.io.led.setDigitalValue(state);
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 1);

        // print the number of seconds since reset:
        lcd.print(system_timer_current_time()/1000);

        iotNode.sleep(1000);
        state = !state;
    }
}