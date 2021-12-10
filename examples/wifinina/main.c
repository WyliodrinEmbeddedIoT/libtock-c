#include <stdio.h>
#include <timer.h>
#include "wifinina.h"

static void scan_networks_cb(int status, int len, int max_len, void *ud)
{
    wifi_networks_t *networks_buffer = (wifi_networks_t *)ud;
    if (status == RETURNCODE_SUCCESS)
    {
        printf("Got %d networks, displaying %d:\n", max_len, len);
        for (int i = 0; i < len; i++)
        {
            printf("%s\n", networks_buffer->networks[i].ssid);
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
