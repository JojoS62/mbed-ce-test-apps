#
# Mbed CE Hello World Project
#
cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

# Initialize Mbed OS build system. 
# Note: This block must be before the project() call.
set(MBED_APP_JSON_PATH mbed_app.json)
set(CUSTOM_TARGETS_JSON_PATH custom_targets/custom_targets.json) # If you need a custom target, use this line to specify the custom_targets.json
set(MBED_ENABLE_TESTING ON)
set(MBED_BUILD_GREENTEA_TESTS ON)
set(MBED_GREENTEA_SERIAL_PORT "/dev/ttyACM0")
set(STM32CUBE_UPLOAD_ENABLED TRUE)
set(UPLOAD_METHOD "STM32CUBE")
set(CMAKE_BUILD_TYPE "Develop")
set(MBED_TARGET "WEACT_F411CE")

include(mbed-os/tools/cmake/app.cmake) # Load Mbed CE toolchain file and basic build system

# If you need any custom upload method configuration for your target, do that here

add_subdirectory(custom_targets)
add_subdirectory(mbed-os) # Load Mbed OS build targets.  Must be added before any other subdirectories

project(MbedCEHelloWorld)
# add_executable(HelloWorld main.cpp)
# target_link_libraries(HelloWorld mbed-os) # Can also link to mbed-baremetal here
# mbed_set_post_build(HelloWorld HelloWorld stm32f411xe.ld) # Must call this for each target to set up bin file creation, code upload, etc
mbed_finalize_build()