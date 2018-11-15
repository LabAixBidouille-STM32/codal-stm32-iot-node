#ifndef STM32_IOT_NODE_H
#define STM32_IOT_NODE_H

#include "CodalComponent.h"
#include "CodalFiber.h"

#include "NotifyEvents.h"
#include "MessageBus.h"

#include "STM32IotNodeIO.h"
#include "STM32SPBTLE.h"
#include "stm32l4xxTimer.h"
#include "stm32l4xxSPI.h"
#include "stm32l4xxI2C.h"

// Status flag values
#define DEVICE_INITIALIZED                    0x01

/**
 * Class definition for a STM32 IOT node.
 *
 * Represents the device as a whole, and includes member variables that represent various device drivers
 * used to control aspects of the STM32 IOT node.
 */
namespace codal
{
    class STM32IotNode : public CodalComponent
    {
        public:
            STM32L4xxTimer timer;
            MessageBus     messageBus;

            STM32IotNodeIO io;

            STM32L4xxSPI spi1;//External on arduino stardard pins
            STM32L4xxSPI spi3;//Internal used by BLE, SubGHz and wifi

            STM32L4xxI2C i2c1;//External on arduino stardard pins 
            STM32L4xxI2C i2c2;//Internal only used by MEMS sensors 

            
            //BLE& ble;
            
            /**
             * Constructor.
             */
            STM32IotNode();

            /**
             * Post constructor initialisation method.
             */
            virtual int init();

            /**
             * Delay execution for the given amount of time.
             *
             * If the scheduler is running, this will deschedule the current fiber and perform
             * a power efficient, concurrent sleep operation.
             *
             * If the scheduler is disabled or we're running in an interrupt context, this
             * will revert to a busy wait.
             *
             * Alternatively: wait, wait_ms, wait_us can be used which will perform a blocking sleep
             * operation.
             *
             * @param milliseconds the amount of time, in ms, to wait for. This number cannot be negative.
             *
             */
            virtual void sleep(uint32_t milliseconds){
                fiber_sleep(milliseconds);
            }

            /**
             * A periodic callback invoked by the fiber scheduler idle thread.
             * We use this for any low priority, background housekeeping.
             */
            virtual void idleCallback();

            /**
             * A periodic callback invoked by the fiber scheduler every SCHEDULER_TICK_PERIOD_MS.
             */
            virtual void periodicCallback();

            /**
             * Determine the time since this board was last reset.
             *
             * @return The time since the last reset, in milliseconds.
             *
             * @note This will value overflow after 1.6 months.
             */
            //TODO: handle overflow case.
            unsigned long systemTime(){
                return system_timer_current_time();
            }
    };
}
#endif
