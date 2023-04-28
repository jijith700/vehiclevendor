# vehiclevendor

Customized VHAL with custom vehicle property.

    RPI4_GPIO

This property is used to update the value of the Raspberry Pi GPIO.<br />
Clone the project and add the below changes in respective files.

**Car.mk**<br />
```shell
PRODUCT_PACKAGES += \
    vendor.demo.vehiclevendor@1.0-service \
    vendor.demo.rpi4gpio@1.0-service
```
    
**BoardConfig.mk**<br />
```xml
BOARD_SEPOLICY_DIRS := \
    vendor/demo/hardware/interfaces/vehiclevendor/1.0/default/sepolicy\
    vendor/demo/hardware/interfaces/rpi4gpio/1.0/default/sepolicy \
```
    
After adding the above changes build the AAOS. The vehicle property will be created.<br />
Add `vendor.demo.vehiclevendor-V1.0-java` in the Android.bp to access the custom vehicle property. Register the vehicle property as:

     private fun registerCar() {
        carPropertyManager = car?.getCarManager(Car.PROPERTY_SERVICE) as CarPropertyManager
        val propertyIds = ArraySet<Int>()
        propertyIds.add(VehicleProperty.RPI4_GPIO)
        val property = carPropertyManager.getIntProperty(VehicleProperty.RPI4_GPIO, 0)
        Log.i(TAG, "registerCar: property= $property")
        carPropertyManager.registerCallback(this, VehicleProperty.RPI4_GPIO, 0.0f)
    }

**Reference** <br />
[RPI HAL](https://github.com/jijith700/rpi4gpio "RPI HAL"):- HAL to access the RPI4 GPIO pin 23.<br />
[Rpi4GpioApp](https://github.com/jijith700/Rpi4GpioApp "Rpi4GpioApp"):- Android App to change the GPIO pin.
