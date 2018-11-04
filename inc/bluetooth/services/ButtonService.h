/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

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

#ifndef CODAL_BUTTON_SERVICE_H
#define CODAL_BUTTON_SERVICE_H

#include "CodalConfig.h"
#include "BLE.h"
#include "EventModel.h"

// UUIDs for our service and characteristics
extern const uint8_t  ButtonServiceUUID[];
extern const uint8_t  ButtonAServiceDataUUID[];
extern const uint8_t  ButtonBServiceDataUUID[];


/**
  * Class definition for a  BLE Button Service.
  * Provides access to live button data via BLE, and provides basic configuration options.
  */
class ButtonService
{
    public:

    /**
      * Constructor.
      * Create a representation of the ButtonService
      * @param _ble The instance of a BLE device that we're running on.
      */
    ButtonService(BLEDevice &_ble);


    private:

    /**
     * Button A update callback
     */
    void buttonAUpdate(Event e);

    /**
     * Button B update callback
     */
    void buttonBUpdate(Event e);

    // Bluetooth stack we're running on.
    BLEDevice           &ble;

    // memory for our 8 bit control characteristics.
    uint8_t            buttonADataCharacteristicBuffer;
    uint8_t            buttonBDataCharacteristicBuffer;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t buttonADataCharacteristicHandle;
    GattAttribute::Handle_t buttonBDataCharacteristicHandle;
};


#endif
