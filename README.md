# vehiclevendor
# Car.mk
PRODUCT_PACKAGES += \\
    vendor.demo.vehiclevendor@1.0-service \\
    vendor.demo.rpi4gpio@1.0-service
    
# BoardConfig.mk
BOARD_SEPOLICY_DIRS := \\
    vendor/demo/hardware/interfaces/vehiclevendor/1.0/default/sepolicy \\
    vendor/demo/hardware/interfaces/rpi4gpio/1.0/default/sepolicy \\
