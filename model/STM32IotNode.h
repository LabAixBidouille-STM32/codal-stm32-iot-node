#ifndef STM32_IOT_NODE_H
#define STM32_IOT_NODE_H

#include "CodalComponent.h"
#include "CodalDevice.h"
#include "CodalFiber.h"
#include "CodalDmesg.h"
#include "codal-core/inc/types/Event.h"
#include "codal-core/inc/driver-models/Timer.h"

#include "NotifyEvents.h"
#include "MessageBus.h"

#include "Button.h"
#include "STM32IotNodeIO.h"
#include "STM32SPBTLE.h"
#include "stm32l4xxLowLevelTimer.h"
#include "stm32l4xxSPI.h"
#include "stm32l4xxI2C.h"
#include "stm32l4xxSimpleSerial.h"
#include "STM32IotNodeTemperature.h"
#include "STM32IotNodeHumidity.h"
#include "STM32IotNodePressure.h"
#include "STM32IotNodeDistance.h"
#include "STM32IotNodeAccelerometer.h"
#include "STM32IotNodeGyroscope.h"
#include "STM32IotNodeMagnetometer.h"

// Status flag values
#define DEVICE_INITIALIZED 0x01

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
            STM32L4xxLowLevelTimer tim5;
            Timer timer;

            MessageBus     messageBus;

            STM32IotNodeIO io;

            STM32L4xxSimpleSerial serial;

            Button buttonUSER;

            STM32L4xxSPI spi1;//External on arduino stardard pins
            STM32L4xxSPI spi3;//Internal used by BLE, SubGHz and wifi

            STM32L4xxI2C i2c1;//External on arduino stardard pins 
            STM32L4xxI2C i2c2;//Internal only used by MEMS sensors 

            CoordinateSpace coordinateSpace;

            STM32IotNodeDistance distance;
            STM32IotNodeTemperature temperature;
            STM32IotNodeHumidity humidity;
            STM32IotNodePressure pressure;
            STM32IotNodeAccelerometer accelerometer;
            STM32IotNodeGyroscope gyroscope;
            STM32IotNodeMagnetometer magnetometer;
            
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
            virtual void idleCallback(){
                codal_dmesg_flush();
            }

            /**
             * A periodic callback invoked by the fiber scheduler every SCHEDULER_TICK_PERIOD_MS.
             */
            virtual void periodicCallback(){
            }

            /**
             * Determine the time since this board was last reset.
             *
             * @return The time since the last reset, in milliseconds.
             *
             * @note This will value overflow after 1.6 months.
             */
            CODAL_TIMESTAMP systemTime(){
                return system_timer_current_time();
            }
    };

    extern STM32IotNode *default_device_instance;
}
#endif
