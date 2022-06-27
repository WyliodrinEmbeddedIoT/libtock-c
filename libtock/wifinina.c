#include "wifinina.h"
#include "tock.h"
#include <stdlib.h>
#include <string.h>

static int scan_subscribe(subscribe_upcall callback, void *callback_args)
{
    subscribe_return_t sval = subscribe(WIFI_SYSCALLS_DRIVER_NUM, 1, callback, callback_args);
    return tock_subscribe_return_to_returncode(sval);
}

static int scan_allow(void *data, int len)
{
    allow_rw_return_t aval = allow_readwrite(WIFI_SYSCALLS_DRIVER_NUM, 0, data, len);
    return tock_allow_rw_return_to_returncode(aval);
}

typedef struct
{
    subscribe_upcall *callback;
    void *ud;
} scan_networks_callback_data_t;

static void scan_networks_internal_callback(int status, int net_len, int actual_net_len, void *ud)
{
    scan_networks_callback_data_t *subscribe_data = (scan_networks_callback_data_t *)ud;
    scan_allow(NULL, 0);
    if (subscribe_data != NULL)
    {
        if (subscribe_data->callback)
        {
            subscribe_data->callback(status, net_len, actual_net_len, subscribe_data->ud);
        }
        free(subscribe_data);
    }
}

wifi_networks_t *wifi_allocate_networks_buffer(size_t cap)
{

    wifi_networks_t *networks_buffer = (wifi_networks_t *)malloc(sizeof(wifi_networks_t));
    if (networks_buffer != NULL)
    {
        networks_buffer->networks = (wifi_network_t *)malloc(cap * sizeof(wifi_network_t));
        if (networks_buffer->networks != NULL)
        {
            networks_buffer->cap = cap;
        }
        else
        {
            free(networks_buffer);
            networks_buffer = NULL;
        }
    }
    return networks_buffer;
}

void wifi_free_networks_buffer(wifi_networks_t **networks_buffer)
{
    free((*networks_buffer)->networks);
    free(*networks_buffer);
    *networks_buffer = NULL;
}

int wifi_scan(wifi_networks_t *networks_buffer, subscribe_upcall scan_callback, void *ud)
{
    int err = RETURNCODE_SUCCESS;
    if (networks_buffer != NULL)
    {
        err = scan_allow(networks_buffer->networks, networks_buffer->cap * sizeof(wifi_network_t));
        if (err == RETURNCODE_SUCCESS)
        {
            scan_networks_callback_data_t *subscribe_data = malloc(sizeof(scan_networks_callback_data_t));
            if (subscribe_data != NULL)
            {
                subscribe_data->callback = scan_callback;
                subscribe_data->ud = ud;
                err = scan_subscribe(scan_networks_internal_callback, subscribe_data);
                if (err == RETURNCODE_SUCCESS)
                {
                    syscall_return_t ret = command(WIFI_SYSCALLS_DRIVER_NUM, 1, 0, 0);
                    if (ret.type == TOCK_SYSCALL_SUCCESS)
                    {
                        return RETURNCODE_SUCCESS;
                    }
                    scan_subscribe(NULL, NULL);
                    err = tock_status_to_returncode(ret.data[0]);
                }
                free(subscribe_data);
            }
            scan_allow(NULL, 0);
        }
    }
    else
    {
        err = RETURNCODE_ENOMEM;
    }
    return err;
}

wifi_networks_t *wifi_scan_sync(size_t max_len, int *status)
{
    wifi_networks_t *networks = wifi_allocate_networks_buffer(max_len);
    wifi_free_networks_buffer(&networks);
    *status = RETURNCODE_FAIL;
    return NULL;
}

typedef struct
{
    subscribe_upcall *callback;
    // void *ud;
} connect_to_network_callback_data_t;

static int connect_to_network_subscribe(subscribe_upcall callback, void *callback_args)
{
    subscribe_return_t sval = subscribe(WIFI_SYSCALLS_DRIVER_NUM, 2, callback, callback_args);
    return tock_subscribe_return_to_returncode(sval);
}

static int connect_allow(void *data, int len)
{
    allow_ro_return_t aval = allow_readonly(WIFI_SYSCALLS_DRIVER_NUM, 0, data, len);
    return tock_allow_ro_return_to_returncode(aval);
}

int connect_to_network(char* ssid, char *psk, subscribe_upcall connect_callback) {
    int total_len = strlen(ssid) + strlen(psk);
    char *network_credentials = malloc(total_len * sizeof(char));
    strcpy(network_credentials, ssid);
    strcat(network_credentials, psk);
    int err = connect_allow(network_credentials, total_len);
    if (err == RETURNCODE_SUCCESS) {
        connect_to_network_callback_data_t *subscribe_data = malloc(sizeof(connect_to_network_callback_data_t));
        if (subscribe_data != NULL)
        {
            subscribe_data->callback = connect_callback;
            err = connect_to_network_subscribe(connect_callback, subscribe_data);
            if (err == RETURNCODE_SUCCESS)
            {
                syscall_return_t ret = command(WIFI_SYSCALLS_DRIVER_NUM, 2, strlen(ssid), strlen(psk));
                if (ret.type == TOCK_SYSCALL_SUCCESS)
                {
                    return RETURNCODE_SUCCESS;
                }
                connect_to_network_subscribe(NULL, NULL);
                err = tock_status_to_returncode(ret.data[0]);
            }
            free(subscribe_data);
        } else {
            err = RETURNCODE_ENOMEM;
        }
    } else {
        err = RETURNCODE_ENOMEM;
    }
    return err;
}