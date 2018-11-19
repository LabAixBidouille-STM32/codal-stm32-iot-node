#include "STM32IotNode.h"
#include "CodalDmesg.h"

using namespace codal;
void STM32IotNode_dmesg_flush();

STM32IotNode *codal::default_device_instance = nullptr;


/**
  * Constructor.
  *
  * Create a representation of a device, which includes member variables
  * that represent various device drivers used to control aspects of the STM32 IOT node.
  */
STM32IotNode::STM32IotNode()
  : CodalComponent(),
    timer(), 
    messageBus(), 
    io(),
    serial(io.tx, io.rx),
    spi1(io.miso, io.mosi, io.sclk), 
    spi3(io.miso3, io.mosi3, io.sclk3),
    i2c1(io.sda, io.scl),
    i2c2(io.sda2, io.scl2)
    //ble(BLE::Instance())
{
    // Clear our status
    status = 0;
    default_device_instance = this;
    codal::default_serial_debug = &this->serial;
    codal::default_i2c_sensors_bus = &this->i2c2;
}

/**
  * Post constructor initialisation method.
  *
  * This call will initialised the scheduler, memory allocator and Bluetooth stack.
  *
  * This is required as the Bluetooth stack can't be brought up in a
  * static context i.e. in a constructor.
  *
  * @code
  * stm32IotNode.init();
  * @endcode
  *
  * @note This method must be called before user code utilises any functionality
  *       contained within the STM32IotNode class.
  */
int STM32IotNode::init()
{
    if (status & DEVICE_INITIALIZED)
        return DEVICE_NOT_SUPPORTED;

    status |= DEVICE_INITIALIZED;

    timer.init();
    serial.init();
    i2c2.init();

    // Bring up fiber scheduler.
    scheduler_init(messageBus);

    for(int i = 0; i < DEVICE_COMPONENT_COUNT; i++)
    {
        if(CodalComponent::components[i]){
            CodalComponent::components[i]->init();
        }
    }

    codal_dmesg_set_flush_fn(STM32IotNode_dmesg_flush);
    status |= DEVICE_COMPONENT_STATUS_IDLE_TICK;

    return DEVICE_OK;
}

/**
  * A periodic callback invoked by the fiber scheduler idle thread.
  * We use this for any low priority, background housekeeping.
  *
  */
void STM32IotNode::idleCallback()
{
    codal_dmesg_flush();
}

void STM32IotNode::periodicCallback(){}

void STM32IotNode_dmesg_flush()
{
#if CONFIG_ENABLED(DMESG_SERIAL_DEBUG)
/*
#if DEVICE_DMESG_BUFFER_SIZE > 0
    if (codalLogStore.ptr > 0 && default_device_instance)
    {
        for (uint32_t i=0; i<codalLogStore.ptr; i++)
            ((STM32IotNode *)default_device_instance)->serial.putc(codalLogStore.buffer[i]);

        codalLogStore.ptr = 0;
    }
#endif
*/
#endif
}

int target_seed_random(uint32_t rand)
{
    return codal::seed_random(rand);
}

int target_random(int max)
{
    return codal::random(max);
}
