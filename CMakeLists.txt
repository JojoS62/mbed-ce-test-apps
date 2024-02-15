#
# Mbed CE Hello World Project
#
cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

# Initialize Mbed OS build system. 
# Note: This block must be before the project() call.
set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(STM32CUBE_CONNECT_COMMAND "-c port=SWD reset=HWrst")
set(STM32CUBE_GDBSERVER_ARGS "--swd")


set(MBED_APP_JSON_PATH mbed_app.json)
set(CUSTOM_TARGETS_JSON_PATH custom_targets/custom_targets.json) # This activates the custom targets in custom_targets.json

include(mbed-os/tools/cmake/app.cmake) # Load Mbed CE toolchain file and basic build system

# If you need any custom upload method configuration for your target, do that here

project(HelloWorld)

add_subdirectory(custom_targets)
add_subdirectory(mbed-os) # Load Mbed OS build targets.  Must be added before any other subdirectories
add_subdirectory(mbed-libs)


add_subdirectory(app_Modbus EXCLUDE_FROM_ALL)
add_subdirectory(app_Hello EXCLUDE_FROM_ALL)
add_subdirectory(app_Ethernet EXCLUDE_FROM_ALL)

mbed_finalize_build()