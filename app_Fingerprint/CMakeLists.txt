# Copyright (c) 2021 Arm Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_executable(Fingerprint main.cpp)

target_link_libraries(Fingerprint 
    mbed-os
    fingerprint-lib
)

mbed_set_post_build(Fingerprint)
