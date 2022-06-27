#pragma once
#include "tock.h"
#define WIFI_SYSCALLS_DRIVER_NUM 0x90004
#ifdef __cplusplus
extern "C"
 {
#endif

enum StationStatus {
    // the device is not a station
    // it might be an access point
    Off,
    // the station is connected to the `Network`
    Connected,
    // the station is in the process of connecting to the `Network`
    Connecting,
    // the station is not connected to any network
    Disconnected,
    // the station is disconnecting from a network
    Disconnecting
};

typedef struct __attribute__((__packed__)) {
    unsigned char ssid[33];
    unsigned char rssi;
    unsigned char security;
} wifi_network_t;

typedef struct {
    wifi_network_t *networks;
    // how many network buffers have been allocated
    size_t cap;
} wifi_networks_t;

// buffer data format
//  0                      33              34              35                 ...
// +---------+-------------+---------------+---------------+---------------------+-----------+ ...
// | ssid (SSID)           | rssi (u8)     | security (u8) |                     |           |        ...
// +---------+-------------+---------------+---------------+---------------------+---------- ...
// | Network 0                                             | Network 1

int wifi_scan (wifi_networks_t *networks_buffer, subscribe_upcall scan_callback, void *ud);
wifi_networks_t * wifi_allocate_networks_buffer(size_t len);
void wifi_free_networks_buffer(wifi_networks_t *networks_buffer[]);

wifi_networks_t *wifi_scan_sync(size_t max_len, int *status);

int connect_to_network(char* ssid, char *psk, subscribe_upcall connect_callback);
    
#ifdef __cplusplus
}
#endif