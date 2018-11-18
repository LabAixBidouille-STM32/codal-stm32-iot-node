#pragma once
#include "CodalConfig.h"
#include "stm32.h"

namespace codal{
    void updateODR(uint8_t odr);
    uint8_t getBestAdaptedODRValue(float& frequency);
}