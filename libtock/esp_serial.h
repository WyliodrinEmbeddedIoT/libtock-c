#pragma once

#include "tock.h"

#define DRIVER_NUM_ESP_SERIAL 0x90009

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* get_tx_buffer (void);
// uint8_t* get_rx_buffer (void); 

int bind_socket (uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, size_t port);
int send_payload (size_t len);
int close_socket (void);

#ifdef __cplusplus
}
#endif