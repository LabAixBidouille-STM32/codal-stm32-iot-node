#ifndef __RGB_LCD_H__
#define __RGB_LCD_H__

#include <inttypes.h>
#include "stm32l4xxI2C.h"

// Device I2C Arress
#define LCD_ADDRESS     (0x7c)
#define RGB_ADDRESS     (0xc4)

// base define
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

// color define 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

namespace codal {
class rgb_lcd 
{
private:
  STM32L4xxI2C& i2c;
  uint8_t cols;
  uint8_t lines; 
  uint8_t dotsize;
  int write_error;

  size_t printNumber(unsigned long, uint8_t);
  void printLLNumber(uint64_t, uint8_t );
  size_t printFloat(double, uint8_t);

protected:
  void setWriteError(int err = 1) { write_error = err; }
  
public:
  rgb_lcd(STM32L4xxI2C& i2c, uint8_t cols, uint8_t lines, uint8_t dotsize = LCD_5x8DOTS);

  void init();

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  
  virtual size_t write(uint8_t);
  
  void command(uint8_t);
  
  // color control
  void setRGB(unsigned char r, unsigned char g, unsigned char b);               // set rgb
  void setPWM(unsigned char color, unsigned char pwm){writeRGBRegister(color, pwm);}      // set pwm
  
  void setColor(unsigned char color);
  void setColorAll(){setRGB(0, 0, 0);}
  void setColorWhite(){setRGB(255, 255, 255);}

  // blink the LED backlight
  void blinkLED(void);
  void noBlinkLED(void);

  int getWriteError() { 
    return write_error; 
  }

  void clearWriteError() { 
    setWriteError(0); 
  }

  size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
  }

  virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
  }

  size_t print(const char[]);
  size_t print(char);
  size_t print(unsigned char, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(double, int = 2);

  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(double, int = 2);
  size_t println(void);

private:
  void send(uint8_t, uint8_t);
  void writeRGBRegister(unsigned char addr, unsigned char dta);

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines;
  uint8_t _currline;
};
}
#endif