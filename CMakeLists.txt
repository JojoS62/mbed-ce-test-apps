#
# Mbed CE Hello World Project
#
cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

# Initialize Mbed OS build system. 
# Note: This block must be before the project() call.
set(MBED_APP_JSON_PATH mbed_app.json)
set(STM32CUBE_UPLOAD_ENABLED  TRUE)

include(mbed-os/tools/cmake/app.cmake) # Load Mbed CE toolchain file and basic build system

# If you need any custom upload method configuration for your target, do that here

add_subdirectory(custom_targets)
add_subdirectory(mbed-os) # Load Mbed OS build targets.  Must be added before any other subdirectories

project(HelloWorld)
add_executable(HelloWorld main.cpp)
target_link_libraries(HelloWorld mbed-os) # Can also link to mbed-baremetal here

mbed_finalize_build()