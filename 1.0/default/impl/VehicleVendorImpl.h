#pragma once

#include <vhal_v2_0/RecurrentTimer.h>
#include <vhal_v2_0/VehicleHal.h>
#include <vhal_v2_0/VehiclePropertyStore.h>

#include "VehicleVendorClient.h"

namespace vhal_v2_0 = android::hardware::automotive::vehicle::V2_0;

namespace vendor {
namespace demo {
namespace vehiclevendor {
namespace V1_0 {

namespace impl {

class VehicleVendorImpl : public vhal_v2_0::VehicleHal
{
public:
    VehicleVendorImpl(vhal_v2_0::VehiclePropertyStore* propStore, VehicleVendorClient* client);
    ~VehicleVendorImpl() = default;

    void onCreate() override;
    std::vector<vhal_v2_0::VehiclePropConfig> listProperties() override;
    VehiclePropValuePtr get(
        const vhal_v2_0::VehiclePropValue& requestedPropValue,
        vhal_v2_0::StatusCode* outStatus) override;
    vhal_v2_0::StatusCode set(const vhal_v2_0::VehiclePropValue& propValue) override;
    vhal_v2_0::StatusCode subscribe(int32_t property, float sampleRate) override;
    vhal_v2_0::StatusCode unsubscribe(int32_t property) override;

private:
    void onPropertyValue(const vhal_v2_0::VehiclePropValue& value, bool updateStatus);
    void regPropsInPropStore();
    void initPropsInPropStore();
    void onContinuousPropertyTimer(const std::vector<int32_t>& properties);
    bool isContinuousProperty(int32_t propId) const;

    vhal_v2_0::VehiclePropertyStore* mPropStore;
    VehicleVendorClient* mVehicleClient;
    RecurrentTimer mRecurrentTimer;
};

} // namespace impl

} // namespace V1_0
} // namespace vehiclevendor
} // namespace demo
} // namespace vendor
