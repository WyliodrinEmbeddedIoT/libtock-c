#include "esp_serial.h"
#include <stdlib.h>

static uint8_t *tx_buffer = NULL;
// static uint8_t *rx_buffer = NULL;
static size_t tx_buffer_len = 0;
// static size_t rx_buffer_len = 0;

int esp_subscribe(subscribe_cb cb, void *userdata, size_t cb_type)
{
    return subscribe(DRIVER_NUM_ESP_SERIAL, cb_type, cb, userdata);
}

int esp_allow(void* ptr, size_t buffer_type, size_t size)
{
    return allow(DRIVER_NUM_ESP_SERIAL, buffer_type, ptr, size);
}

int esp_command(int command_num, size_t data1, size_t data2)
{
    return command(DRIVER_NUM_ESP_SERIAL, command_num, data1, data2);
}


int bind_socket (uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, size_t port)
{
    if (tx_buffer != NULL) {
        return TOCK_EALREADY;
    } else {
        tx_buffer = (uint8_t*) malloc (1024);
        if (tx_buffer != NULL) {
            tx_buffer_len = 1024;
            int ret = esp_allow(tx_buffer, 1, 1024);
        } else {
            return TOCK_FAIL;
        }
    }
    uint32_t ip_value = 0;
    ip_value |= ip1;
    // printf("%lu\n", ip_value);
    ip_value <<= 8;
    // printf("%lu\n", ip_value);
    ip_value |= ip2;
    // printf("%lu\n", ip_value);
    ip_value <<= 8;
    // printf("%lu\n", ip_value);
    ip_value |= ip3;
    // printf("%lu\n", ip_value);
    ip_value <<= 8;
    // printf("%lu\n", ip_value);
    ip_value |= ip4;
    // printf("%lu\n", ip_value);
    return esp_command(1, ip_value, port);
}

int send_payload (size_t len)
{
    return esp_command(2, len, 0);
}

int close_socket (void)
{
    return esp_command(5, 0, 0);
}

uint8_t* get_tx_buffer (void)
{
    return tx_buffer;
}