#include "network.h"
#include <cstdio>

NetworkInterface* network;

void network_init() {
    // Connect to the network with the default networking interface
    // if you use WiFi: see mbed_app.json for the credentials
    printf("starting network\n");
    network = NetworkInterface::get_default_instance();
    if (!network) {
        printf("Cannot connect to the network, see serial output\n");
        return;
    } 
    nsapi_error_t connect_status = network->connect();

    if (connect_status != NSAPI_ERROR_OK) {
        printf("Failed to connect to network (%d)\n", connect_status);
        return;
    } else {
        SocketAddress socketAddress;
        network->get_ip_address(&socketAddress);
        printf("network is up, IP:  %s\n", socketAddress.get_ip_address());     
    }
}
