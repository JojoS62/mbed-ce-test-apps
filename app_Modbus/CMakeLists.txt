#
# Mbed CE Hello World Project
#

# If you need any custom upload method configuration for your target, do that here

add_executable(TestModbus 
    main.cpp
    modbus_slave.cpp)
    
target_link_libraries(TestModbus 
    mbed-os
    mbed-modbus) # Can also link to mbed-baremetal here

mbed_set_post_build(TestModbus) # Must call this for each target to set up bin file creation, code upload, etc
