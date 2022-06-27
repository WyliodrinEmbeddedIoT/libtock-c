#include <stdio.h>
#include <timer.h>
#include <string.h>
#include "wifinina.h"

char *ssid = "Valex";
char *psk = "iubescpepsi69";

static void connect_to_network_cb(int status, int arg1, int arg2) {
    if (status == Connected) {
        printf("Connected to SSID: %s succesfully!\n", ssid);
    } else if (status == Disconnected) {
        printf("Connecting to %s failed!\n", ssid);
    } else {
        printf("Connection failed! Other status: %d\n", status);
    }
}

static void scan_networks_cb(int status, int len, int max_len, void *ud)
{
    wifi_networks_t *networks_buffer = (wifi_networks_t *)ud;
    if (status == RETURNCODE_SUCCESS)
    {
        printf("Got %d networks, displaying %d:\n", max_len, len);
        for (int i = 0; i < len; i++)
        {
            printf("%s\n", networks_buffer->networks[i].ssid);
            if (strcmp(networks_buffer->networks[i].ssid, ssid) == 0) {
                printf("^ ----Found the wifi!----");
                connect_to_network(ssid, psk, connect_to_network_cb);
            }
        }
    }
    else
    {
        printf("Failed to get networks: %s\n", tock_strerr(status));
    }
    wifi_free_networks_buffer(&networks_buffer);
}

int main(void)
{
    delay_ms(2000);
    wifi_networks_t *networks_buffer = wifi_allocate_networks_buffer(20);
    if (wifi_scan(networks_buffer, scan_networks_cb, networks_buffer) != RETURNCODE_SUCCESS)
    {
        printf("Failed to scan networks\n");
    }
}
