// Based on hardware/interfaces/automotive/vehicle/2.0/default/impl/vhal_v2_0/VehicleHalClient.cpp

#define LOG_TAG __FILE_NAME__
#include <log/log_main.h>

#include "VehicleVendorClient.h"

#include <android-base/logging.h>

namespace vendor {
namespace demo {
namespace vehiclevendor {
namespace V1_0 {

namespace impl {

void VehicleVendorClient::onPropertyValue(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    if (!mPropCallback)
    {
        ALOGE("RPI4_VHAL %s(): PropertyCallBackType is not registered!", __func__);
        return;
    }
    return mPropCallback(value, updateStatus);
}

void VehicleVendorClient::registerPropertyValueCallback(PropertyCallBackType&& callback)
{
    if (mPropCallback)
    {
        ALOGW("RPI4_VHAL %s(): Cannot register multiple callbacks!", __func__);
        return;
    }
    mPropCallback = std::move(callback);
}

} // namespace impl

} // namespace V1_0
} // namespace vehiclevendor
} // namespace demo
} // namespace vendor
