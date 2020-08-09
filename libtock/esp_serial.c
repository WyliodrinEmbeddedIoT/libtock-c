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

int check_response()
{
    // todo
    return 0;
}

int send_command (int command_num)
{
    CallbackReturn cbret;
    cbret.done = false;
    esp_subscribe (write_callback, &cbret, 1);
    cbret.error = esp_command(command_num, 64, 0);
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    esp_subscribe (NULL, NULL, 1);
    return cbret.error;
}

int receive_command (int type)
{
    CallbackReturn cbret;
    int ret;
    cbret.done = false;
    esp_subscribe (read_callback, &cbret, 2);
    yield_for (&cbret.done);
    esp_subscribe(NULL, NULL, 2);
    if (cbret.error == TOCK_SUCCESS) {
        if (type) {
            // get ip and save it in rx_buffer
            return 0;
        } else return check_response();
    } else return cbret.error;
}

int connect_to_wifi (char* ssid, char* password)
{
    if (tx_buffer != NULL) {
        return TOCK_EALREADY;
    } else {
        tx_buffer = (uint8_t*) calloc (64, sizeof(uint_fast8_t));
        if (tx_buffer != NULL) {
            tx_buffer_len = 64;
            int ret = esp_allow(tx_buffer, 1, 64);
        } else {
            return TOCK_FAIL;
        }
    }
    memset(tx_buffer, 0, 64);
    sprintf(tx_buffer, "%s\n\r", SET_WIFI_COMMAND);
    int ret = send_command(2);
    
    if (ret == TOCK_SUCCESS) {
        // ret = receive_command (0);
        if (ret == TOCK_SUCCESS) {
            memset(tx_buffer, 0, 64);
            sprintf(tx_buffer, "%s\"%s\",\"%s\"\n\r", WF_COMMAND, ssid, password);
            ret = send_command(2);
            // if (ret == TOCK_SUCCESS) {
                // return receive_command(0);
            // }
        }
    }
    return ret;
}


int bind_socket (char* ip_address, size_t port)
{
    memset(tx_buffer, 0, 64);
    sprintf(tx_buffer, "%s,\"%s\",%d\r\n", START_COMMAND, ip_address, port);
    int ret = send_command(1);
    // if (ret == TOCK_SUCCESS) {
    //     return receive_command(0);
    // }
    return ret;
}

int send_UDP_payload (size_t len, char* str)
{
    memset(tx_buffer, 0, 64);
    sprintf(tx_buffer, "%s%d\r\n%s\r\n", SEND_COMMAND, len, str);
    return send_command(2);
}

int get_esp_ip (void)
{
    memset(tx_buffer, 0, 64);
    snprintf(tx_buffer, 11, "%s\r\n", GET_IP_COMMAND);
    int ret = send_command (2);
    if (ret == TOCK_SUCCESS) {
        return receive_command(1);
    } else return ret;
}

int fake_receive (void)
{
    CallbackReturn cbret;
    cbret.done = false;
    esp_subscribe (read_callback, &cbret, 2);
    cbret.error = esp_command(6, 0, 0);
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    return cbret.error;
}

int close_socket (void)
{
    memset(tx_buffer, 0, 64);
    sprintf(tx_buffer, "%s\r\n", CLOSE_COMMAND);
    return esp_command(5, 64, 0);
}

uint8_t* get_tx_buffer (void)
{
    return tx_buffer;
}