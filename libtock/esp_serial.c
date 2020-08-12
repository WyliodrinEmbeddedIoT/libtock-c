#include "esp_serial.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

// Callback Struct Type
typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} CallbackReturn;

static user_callback *receive_buffer_callback = NULL;
static bool read_cb = false;
static bool write_cb = false;

/* write_callback() defines the callback function that will be called
 * from the kernel after an UDP payload is being sent. 
 * 
 * The arguments are: 
 *  - error (or the return info) can be
 *      * TOCK_SUCCESS
 *      * any error received by the capsule
 *  - data1 -> stores the number of bytes that were sent by the capsule
 *             (in case of an TOCK_SUCCESS return), or 0 (in case of an
 *             actual error)
 */
static void write_callback(int error,
                           int data1,
                           __attribute__ ((unused)) int data2,
                           void* ud) {
  CallbackReturn *ret = (CallbackReturn*)ud;
  ret->error = error;
  ret->data1 = data1;
  ret->done  = true;
}

/* read_callback() defines the callback function that will be called
 * from the kernel when a message for the application was received. The
 * message from the kernel can be found in the shared `rx_buffer`. If
 * the user defined a callback of type `static user_callback*` callback
 * in the main() function, that callback will actually be called from the
 * library's read_callback. 
 * 
 * The arguments are: 
 *  - error (or the return info) can be
 *      * TOCK_SUCCESS
 *      * TOCK_FAIL if there was an `ERROR` response from the ESP
 *      * TOCK_EALREADY if there was en `ALREADY CONNECTED` response
 *  - data1 -> stores the number of bytes that were sent by the capsule
 *             to the userspace application (in case of an TOCK_SUCCESS 
 *             return), or 0 (in case of an actual error)
 */
static void read_callback(int error,
                          int data1,
                          __attribute__ ((unused)) int data2,
                          void* ud) {
  CallbackReturn *ret = (CallbackReturn*)ud;
  ret->error = error;
  ret->data1 = data1;
  ret->done  = true;
  if (error == 0 && data1 != 0) {
    memset(user_buffer, 0, 64);
    memcpy(user_buffer, rx_buffer, data1);
    memset(rx_buffer, 0, 64);
    if (receive_buffer_callback) receive_buffer_callback ((char*) user_buffer, data1);
  }
}

/* Sends to the kernel a subscribe syscall */
int esp_subscribe(subscribe_cb cb, void *userdata, size_t cb_type)
{
    return subscribe(DRIVER_NUM_ESP_SERIAL, cb_type, cb, userdata);
}

/* Sends to the kernel an allow syscall */
int esp_allow(void* ptr, size_t buffer_type, size_t size)
{
    return allow(DRIVER_NUM_ESP_SERIAL, buffer_type, ptr, size);
}

/* Sends to the kernel a command syscall */
int esp_command(int command_num, size_t data1, size_t data2)
{
    return command(DRIVER_NUM_ESP_SERIAL, command_num, data1, data2);
}

/* Sends to the kernel a user defined subscribe syscall */
int subscribe_user_callback (user_callback* cb, void* ud)
{
    receive_buffer_callback = cb;
    return esp_subscribe(cb != NULL ? read_callback : NULL, ud, 2);
}

/* Helper library function that sends any command syscall to the 
 * capsule and waits (in the yield_for()) for the capsule to send a
 * response (via a write callback). 
 *
 * As arguments, there are:
 *      - the command_num for the `command` syscall
 *      - the link_id to identify the communication channel
 */
int send_command (int command_num, int link_id)
{
    CallbackReturn cbret;
    cbret.done = false;
    if (!write_cb) {
        // register the write_callback to be called
        esp_subscribe (write_callback, &cbret, 1);
    }
    // execute command
    cbret.error = esp_command(command_num, strlen((char*)tx_buffer), link_id);
    // wait for the callback to be called
    if (cbret.error == TOCK_SUCCESS) yield_for(&cbret.done);
    // unsubscribe the callback
    // esp_subscribe (NULL, NULL, 1);
    // return the syscall return info
    return cbret.error;
}

/* Helper library function that subscribes a read callback, in order
 * to receive data later from the capsule via the `rx_buffer`. 
 * capsule and waits (in the yield_for()) for the capsule to send a
 * response (via a write callback). 
 *
 * As argument, there is:
 *      - wait_yield: 0 -> just register the callback and return
                      1 -> wait for an immediate response from the capsule
                           (for example wait for the IP address)
 */
int receive_command (int wait_yield)
{
    CallbackReturn cbret;
    cbret.done = false;
    // subscribe the read callback
    if (!read_cb) {
        esp_subscribe (read_callback, &cbret, 2);
        read_cb = true;
    }
    // wait for the callback to be called if necessary
    if (wait_yield) {
        yield_for (&cbret.done);
    }

    // if (cbret.error == TOCK_SUCCESS) {
    //     // are we waiting for some special string? (like ip_address)
    //     if (type) {
    //         // get ip and save it in rx_buffer
    //         return 0;
    //         // else - we wait for data in rx_buffer, we check rx_buffer then
    //     } else return check_response();
    //     // else we were waiting for confirmation (OK, EROOR, etc);
    // } else 
    return cbret.error;
}

/* still in dev */
// int connect_to_wifi (char* ssid, char* password, int link_id)
// {
//     // share tx_buffer, we assume we connect to the internet first
//     if (tx_buffer != NULL) {
//         return TOCK_EALREADY;
//     } else {
//         tx_buffer = (uint8_t*) calloc (64, sizeof(uint_fast8_t));
//         if (tx_buffer != NULL) {
//             tx_buffer_len = 64;
//             int ret = esp_allow(tx_buffer, 1, 64);
//             if (ret != TOCK_SUCCESS) return ret;
//         } else {
//             return TOCK_FAIL;
//         }
//     }
//     // send the connection to wifi command
//     memset(tx_buffer, 0, 64);
//     snprintf((char*) tx_buffer, 14, "%s\n\r", SET_WIFI_COMMAND);
//     int ret = send_command(GENERAL_COMMAND_NUMBER, link_id);
    
//     if (ret == TOCK_SUCCESS) {
//         memset(tx_buffer, 0, 64);
//         // send the actual command with ssid and password
//         snprintf((char*) tx_buffer, 17 + strlen(ssid) + strlen(password), "%s\"%s\",\"%s\"\n\r", WF_COMMAND, ssid, password);
//         ret = send_command(GENERAL_COMMAND_NUMBER, link_id);
//     }
//     return ret;
// }


/* bind() is the function that connects the device to the internet. First
 * it asks the capsule for the first link id available (the capsule supports
 * multiple connections). After that, it send the actual command to connect.
 *
 * As arguments, there are:
 *      - the destination IP saved as a const char*
 *      - the destination port
 *      - the source port (for the host) - if it is 0, it will be assigned a
 *        random value
 *      - the link_id, whose value will be given from the capsule
 */
int bind (const char* ip_address, int port_dest, int* port_src, int* link_id)
{
    int ret;
    if (tx_buffer != NULL) {
        return TOCK_EALREADY;
    } else {
        tx_buffer = (uint8_t*) calloc (64, sizeof(uint_fast8_t));
        if (tx_buffer != NULL) {
            tx_buffer_len = 64;
            ret = esp_allow(tx_buffer, 1, 64);
            if (ret != TOCK_SUCCESS) return ret;
        } else {
            return TOCK_FAIL;
        }
    }

    if (rx_buffer != NULL) {
        return TOCK_EALREADY;
    } else {
        rx_buffer = (uint8_t*) calloc (64, sizeof(uint_fast8_t));
        user_buffer = (uint8_t*) calloc (64, sizeof(uint_fast8_t));
        if (rx_buffer != NULL) {
            rx_buffer_len = 64;
            ret = esp_allow(rx_buffer, 2, 64);
            if (ret != TOCK_SUCCESS) return ret;
        } else {
            return TOCK_FAIL;
        }
    }

    // get first available link_id from capsule (successWithValue)
    *link_id = esp_command(LINK_ID_COMMAND_NUMBER, 0, 0); 
    // send CIPMUX=1 command to enable multiple connections
    memset(tx_buffer, 0, 64);
    snprintf((char*) tx_buffer, 14, "%s\r\n", CONNECTION_TYPE_COMMAND);
    ret = send_command(BIND_COMMAND_NUMBER, *link_id);

    if (ret == TOCK_SUCCESS) {
        // send CIPSTART_COMMAND
        memset(tx_buffer, 0, 64);
        if (*port_src == 0) {
            *port_src = rand() % 5000;
        }
        snprintf((char*) tx_buffer, 35 + strlen(ip_address), "%s%d,\"UDP\",\"%s\",%d,%d\r\n", START_COMMAND, *link_id, ip_address, port_dest, *port_src);
        // still dunno what we're waiting for here?
        ret = send_command(BIND_COMMAND_NUMBER, *link_id);
        if (ret == 0) {
            return receive_command(0);
        }
    }
    return ret;
}


/* send_UPD_payload() saved the message from the user and sends it to the
 * capsule, through a syscall and the shared `tx_buffer`
 */
int send_UDP_payload (size_t len, const char* str, int link_id)
{
    memset(tx_buffer, 0, 64);
    // save the payload and send the command
    snprintf((char*) tx_buffer, 20 + len, "%s%d,%d\r\n%s\r\n", SEND_COMMAND, link_id, len, str);
    return send_command(GENERAL_COMMAND_NUMBER, link_id);
}

/* get_esp_ip() asks the device to send back its ip address. The response will be
 * stored in the `rx_buffer` after the `read_callback`.
 *
 * Demo function only!!
 */
int get_esp_ip (void)
{
    memset(tx_buffer, 0, 64);
    snprintf((char*) tx_buffer, 13, "%s\r\n", GET_IP_COMMAND);
    int ret = send_command (GENERAL_COMMAND_NUMBER, 0);
    if (ret == TOCK_SUCCESS) {
        return receive_command(1);
    }
    return ret;
}

int close_socket (int link_id)
{
    memset(tx_buffer, 0, 64);
    snprintf((char*) tx_buffer, 16, "%s\r\n", CLOSE_COMMAND);
    return esp_command(CLOSE_COMMAND_NUMBER, 64, link_id);
}

uint8_t* get_tx_buffer (void)
{
    return tx_buffer;
}

uint8_t* get_rx_buffer (void)
{
    return rx_buffer;
}