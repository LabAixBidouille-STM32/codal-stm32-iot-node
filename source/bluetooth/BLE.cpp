#include "stm32.h"
#include "BLE.h"
#include "BLEInstanceBase.h"

ble_error_t
BLE::initImplementation(FunctionPointerWithContext<InitializationCompleteCallbackContext*> callback)
{
    ble_error_t err = transport->init(instanceID, callback);
    if (err != BLE_ERROR_NONE) {
        return err;
    }
    return BLE_ERROR_NONE;
}

/**
 * BLE::Instance() and BLE constructor rely upon a static array of initializers
 * to create actual BLE transport instances. A description of these instances
 * and initializers is supposed to be put in some .json file contributing to
 * yotta's configuration (typically in the target definition described by
 * target.json). Here's a sample:
 *
 *  "config": {
 *    ...
 *    "ble_instances": {
 *      "count": 1,
 *      "0" : {
 *        "initializer" : "createBLEInstance"
 *      }
 *    }
 *    ...
 *  }
 *
 * The following macros result in translating the above config into a static
 * array: instanceConstructors.
 */
#if !defined(INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS)
/*
 * The following applies when building without yotta. By default BLE_API provides
 * a trivial initializer list containing a single constructor: createBLEInstance.
 * This may be overridden.
 */
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS createBLEInstance
#endif

typedef BLEInstanceBase *(*InstanceConstructor_t)(void);
static const InstanceConstructor_t instanceConstructors[BLE::NUM_INSTANCES] = {
    INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS
};

BLE &
BLE::Instance(InstanceID_t id)
{
    static BLE *singletons[NUM_INSTANCES];
    if (id < NUM_INSTANCES) {
        if (singletons[id] == NULL) {
            singletons[id] = new BLE(id); /* This object will never be freed. */
        }

        return *singletons[id];
    }

    /* we come here only in the case of a bad interfaceID. */
    static BLE badSingleton(NUM_INSTANCES /* this is a bad index; and will result in a NULL transport. */);
    return badSingleton;
}

#ifdef YOTTA_CFG_MBED_OS
void defaultSchedulingCallback(BLE::OnEventsToProcessCallbackContext* params) {
    minar::Scheduler::postCallback(&params->ble, &BLE::processEvents);
}
#else
#define defaultSchedulingCallback NULL
#endif


BLE::BLE(InstanceID_t instanceIDIn) : instanceID(instanceIDIn), transport(),
    whenEventsToProcess(defaultSchedulingCallback)
{
    static BLEInstanceBase *transportInstances[NUM_INSTANCES];

    if (instanceID < NUM_INSTANCES) {
        if (!transportInstances[instanceID]) {
            transportInstances[instanceID] = instanceConstructors[instanceID](); /* Call the stack's initializer for the transport object. */
        }
        transport = transportInstances[instanceID];
    } else {
        transport = NULL;
    }
}

bool BLE::hasInitialized(void) const
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->hasInitialized();
}

ble_error_t BLE::shutdown(void)
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->shutdown();
}

const char *BLE::getVersion(void)
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getVersion();
}

const Gap &BLE::gap() const
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGap();
}

Gap &BLE::gap()
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGap();
}

const GattServer& BLE::gattServer() const
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGattServer();
}

GattServer& BLE::gattServer()
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGattServer();
}

const GattClient& BLE::gattClient() const
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGattClient();
}

GattClient& BLE::gattClient()
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getGattClient();
}

const SecurityManager& BLE::securityManager() const
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getSecurityManager();
}

SecurityManager& BLE::securityManager()
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    return transport->getSecurityManager();
}

void BLE::waitForEvent(void)
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    transport->waitForEvent();
}

void BLE::processEvents()
{
    if (!transport) {
        codal_error("bad handle to underlying transport");
    }

    transport->processEvents();
}

void BLE::onEventsToProcess(const BLE::OnEventsToProcessCallback_t& callback)
{
    whenEventsToProcess = callback;
}

void BLE::signalEventsToProcess()
{
    if (whenEventsToProcess) {
        OnEventsToProcessCallbackContext params = {
            *this
        };
        whenEventsToProcess(&params);
    }
}