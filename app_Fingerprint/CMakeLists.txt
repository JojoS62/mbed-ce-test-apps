add_executable(Fingerprint 
    main.cpp
    storage.cpp
    )

target_link_libraries(Fingerprint 
    mbed-os
    fingerprint-lib
    mbed-storage-sdio
    mbed-storage
    )

mbed_set_post_build(Fingerprint)
