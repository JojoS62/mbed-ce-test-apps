# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_executable(TestEthernet 
    main.cpp
    network.cpp
    mqtt.cpp
)

target_link_libraries(TestEthernet 
        mbed-os
        mbed-netsocket
        mbed-mqtt
    )

mbed_set_post_build(TestEthernet)
