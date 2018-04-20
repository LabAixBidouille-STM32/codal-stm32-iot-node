/*
The MIT License (MIT)

Copyright (c) 2018 Paul ADAM, inidinn.com

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>

#include "CodalConfig.h"
#include "STM32IotNodeI2C.h"

namespace codal
{

int STM32IotNodeI2C::read( uint32_t address, uint8_t reg, uint8_t* data, uint32_t len )
{
 I2C_HandleTypeDef* Handle = ( I2C_HandleTypeDef* ) &_i2c.i2c.handle;
// printf( " \nSTM32IotNodeI2C::read\n" );
// printf( "  Handle          = 0x%08X\n", ( unsigned int ) Handle );
// printf( "  address         = 0x%08X\n", ( unsigned int ) address );
// printf( "  reg             = 0x%02X\n", ( unsigned int ) reg );
// printf( "  len             = 0x%04X\n", ( unsigned int ) len );
// printf( "  data            = 0x%08X\n", ( unsigned int ) data );
 if ( HAL_I2C_Mem_Read( Handle, address | 1, ( uint16_t ) reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000 ) == HAL_OK )
 {
//  for ( uint32_t i = 0; i < len; i ++ )
//   printf( "   data[ %2u ]     = 0x%02X\n", ( unsigned int ) i, ( unsigned int ) data[ i ] );
  return DEVICE_OK;
 }
 return DEVICE_I2C_ERROR;
}

int STM32IotNodeI2C::write( uint32_t address, uint8_t reg, uint8_t* data, uint32_t len )
{
 I2C_HandleTypeDef* Handle = ( I2C_HandleTypeDef* ) &_i2c.i2c.handle;
// printf( " \nSTM32IotNodeI2C::write\n" );
// printf( "  Handle          = 0x%08X\n", ( unsigned int ) Handle );
// printf( "  address         = 0x%08X\n", ( unsigned int ) address );
// printf( "  reg             = 0x%02X\n", ( unsigned int ) reg );
// printf( "  len             = 0x%04X\n", ( unsigned int ) len );
// printf( "  data            = 0x%08X\n", ( unsigned int ) data );
// for ( uint32_t i = 0; i < len; i ++ )
//  printf( "   data[ %2u ]     = 0x%02X\n", ( unsigned int ) i, ( unsigned int ) data[ i ] );
 if ( HAL_I2C_Mem_Write( Handle, address, ( uint16_t ) reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000 ) == HAL_OK )
  return DEVICE_OK;
 return DEVICE_I2C_ERROR;
}

}
