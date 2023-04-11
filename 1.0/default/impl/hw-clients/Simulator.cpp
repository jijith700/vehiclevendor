#define LOG_TAG __FILE_NAME__
#include <log/log_main.h>

#include <android-base/properties.h>
#include <utils/SystemClock.h>

#include "Simulator.h"
#include "vendor/demo/vehiclevendor/1.0/types.h"

namespace {
const char* kVendorTestSysPropKey = "debug.vendor.demo.RPI4_GPIO";
}

namespace vendor {
namespace demo {
namespace vehiclevendor {
namespace V1_0 {

namespace impl {

Simulator::Simulator()
    : Runnable(std::bind(&Simulator::routine, this, _1))
{
    mVendorTestSysProp.first = {
        .prop = static_cast<int32_t>(VehicleProperty::RPI4_GPIO),
        .timestamp = android::elapsedRealtimeNano(),
        .areaId = 0,
        .status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE,
        .value.int32Values{0}};
}

std::vector<vhal_v2_0::VehiclePropConfig> Simulator::getAllPropertyConfig() const
{
    return std::vector<vhal_v2_0::VehiclePropConfig>();
}

vhal_v2_0::StatusCode Simulator::setProperty(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    ALOGD("RPI4_VHAL %s(): prop: %d", __func__, value.prop);

    vhal_v2_0::StatusCode ret = vhal_v2_0::StatusCode::OK;

    ret = setIfSupportedProperty(value, updateStatus);

    if (ret == vhal_v2_0::StatusCode::OK)
    {
        onPropertyValue(value, updateStatus);
    }

    return ret;
}

void Simulator::routine(const std::atomic_bool& request_to_stop)
{
    vhal_v2_0::VehiclePropValue value;

    for (;;)
    {
        if (request_to_stop) break;

        simulateRpiGpio();

        std::this_thread::sleep_for(100ms);
    }
}

vhal_v2_0::StatusCode Simulator::setIfSupportedProperty(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    vhal_v2_0::StatusCode ret = vhal_v2_0::StatusCode::OK;

    switch (value.prop)
    {
    case static_cast<int32_t>(VehicleProperty::RPI4_GPIO):
        ret = setVendorTestSysProp(value, updateStatus);
        break;
    default:
        break;
    }

    return ret;
}

vhal_v2_0::StatusCode Simulator::setVendorTestSysProp(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    static_cast<void>(updateStatus);
    static_cast<void>(value);

    ALOGE("RPI4_VHAL %s(): prop: %d, try to set READ property!", __func__, value.prop);

    return vhal_v2_0::StatusCode::ACCESS_DENIED;
}

void Simulator::simulateRpiGpio()
{
    int32_t cur_value;

    {
        const std::lock_guard<std::mutex> lock(mVendorTestSysProp.second);
        cur_value = mVendorTestSysProp.first.value.int32Values[0];
    }

    int32_t new_value = android::base::GetIntProperty(
        kVendorTestSysPropKey,
        cur_value,
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::max());

    if (new_value != cur_value)
    {
        vhal_v2_0::VehiclePropValue value;
        {
            const std::lock_guard<std::mutex> lock(mVendorTestSysProp.second);
            mVendorTestSysProp.first.value.int32Values[0] = new_value;
            mVendorTestSysProp.first.timestamp = android::elapsedRealtimeNano();
            mVendorTestSysProp.first.status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE;

            value = mVendorTestSysProp.first;
        }
        onPropertyValue(value, true);
    }
}

} // namespace impl

} // namespace V1_0
} // namespace vehiclevendor
} // namespace demo
} // namespace vendor
