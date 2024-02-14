# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_executable(TestEthernet main.cpp)

target_link_libraries(TestEthernet mbed-os)
target_link_libraries(TestEthernet mbed-netsocket)

mbed_set_post_build(TestEthernet)
