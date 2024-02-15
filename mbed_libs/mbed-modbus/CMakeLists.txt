add_library(mbed-modbus STATIC
        mb.cpp
        mbcrc.cpp
        mbfunccoils.cpp
        mbfuncdiag.cpp
        mbfuncdisc.cpp
        mbfuncholding.cpp
        mbfuncinput.cpp
        mbfuncother.cpp
        mbrtu.cpp
        mbutils.cpp
        portevent.cpp
        portserial.cpp
        porttimer.cpp
)

target_include_directories(mbed-modbus
    INTERFACE
        .
)

target_link_libraries(mbed-modbus 
    PUBLIC 
    mbed-core-flags
) # This is needed so that the library can access Mbed includes

