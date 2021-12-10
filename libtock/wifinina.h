#pragma once
#include "tock.h"
#define WIFI_SYSCALLS_DRIVER_NUM 0x90004
#ifdef __cplusplus
extern "C"
 {
#endif

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
    
#ifdef __cplusplus
}
#endif