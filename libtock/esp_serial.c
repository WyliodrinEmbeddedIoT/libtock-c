#include "esp_serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

static uint8_t *tx_buffer = NULL;
// static uint8_t *rx_buffer = NULL;
static size_t tx_buffer_len = 0;
// static size_t rx_buffer_len = 0;

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} CallbackReturn;

static void write_callback(int error,
                            __attribute__ ((unused)) int data1,
                            __attribute__ ((unused)) int data2,
                            void* ud) {
  CallbackReturn *ret = (CallbackReturn*)ud;
  ret->error = error;
//   ret->data1 = data1;
  ret->done  = true;
}

static void read_callback(int error,
                          int data1,
                          __attribute__ ((unused)) int data2,
                          void* ud) {
  CallbackReturn *ret = (CallbackReturn*)ud;
  ret->error = error;
  ret->data1 = data1;
  ret->done  = true;
}

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
    sprintf(tx_buffer, "%s,\"%d.%d.%d.%d\",%d\0", START_COMMAND, ip1, ip2, ip3, ip4, port);
    CallbackReturn cbret;
    cbret.done = false;
    esp_subscribe (write_callback, &cbret);
    cbret.error = esp_command(1, strlen(tx_buffer), 0);
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    return cbret.error;
}

int send_UDP_payload (size_t len, char* str)
{
    CallbackReturn cbret;
    cbret.done = false;
    esp_subscribe (write_callback, &cbret);
    sprintf(tx_buffer, "%s%d\n%s", SEND_COMMAND, len, str);
    cbret.error = esp_command(2, strlen(tx_buffer), 0);
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    return cbret.error;
}

int fake_receive (void)
{
    CallbackReturn cbret;
    cbret.done = false;
    esp_subscribe (read_callback, &cbret);
    cbret.error = esp_command(6, 0, 0);
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    return cbret.error;
}

int close_socket (void)
{
    sprintf(tx_buffer, "%s\0", CLOSE_COMMAND);
    return esp_command(5, strlen(tx_buffer), 0);
}

uint8_t* get_tx_buffer (void)
{
    return tx_buffer;
}