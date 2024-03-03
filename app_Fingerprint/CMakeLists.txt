add_executable(Fingerprint 
    main.cpp
    storage.cpp
    HTTPHandlers.cpp
    WebsocketHandlers.cpp
    network.cpp
    )

target_link_libraries(Fingerprint 
    mbed-os
    mbed-netsocket
    fingerprint-lib
    mbed-storage-sdio
    mbed-storage
    mbed-http-server
    mbed-tftpd 
    )

mbed_set_post_build(Fingerprint F407.ld)
