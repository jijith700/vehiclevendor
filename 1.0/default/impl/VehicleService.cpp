#define LOG_TAG __FILE_NAME__
#include <log/log_main.h>

#include <chrono>
#include <thread>

#include <hidl/HidlTransportSupport.h>
#include <vhal_v2_0/VehicleHalManager.h>

#include "VehicleVendorImpl.h"
#include "hw-clients/Simulator.h"

using namespace std::chrono_literals;

using namespace vendor::demo::vehiclevendor::V1_0;

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;

int main(int argc, char const* argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    ALOGI("RPI4_VHAL %s(): RPI4 VHAL init started\n", __func__);

    auto store = std::make_unique<vhal_v2_0::VehiclePropertyStore>();

    // HAL client where car signals (e.g. CAN) are transformed to VHAL properties and vice versa.
    // Now Simulator is used, where VENDOR properties are simulated only,
    // Google's SYSTEM properties are dummy and have only init values from DefaultConfig.h.
    auto hw_client = std::make_unique<impl::Simulator>();
    hw_client->start();

    auto hal = std::make_unique<impl::VehicleVendorImpl>(store.get(), hw_client.get());
    auto service = std::make_unique<VehicleHalManager>(hal.get());

    configureRpcThreadpool(1, true); // TODO: to understand

    ALOGI("RPI4_VHAL %s(): Registering as service...", __func__);
    status_t status = service->registerAsService();

    if (status != OK)
    {
        ALOGE("RPI4_VHAL %s(): Unable to register vehicle service (%d)", __func__, status);
        return 1;
    }

    ALOGI("RPI4_VHAL %s(): Vehicle service ready", __func__);
    joinRpcThreadpool();

    return 0;
}
