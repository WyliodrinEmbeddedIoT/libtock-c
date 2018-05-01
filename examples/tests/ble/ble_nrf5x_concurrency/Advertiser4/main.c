#include <ble.h>
#include <gap.h>
#include <stdbool.h>
#include <stdio.h>
#include <tock.h>

int main(void) {
  static uint8_t adv_data_buf[ADV_DATA_MAX_SIZE];
  AdvData_t adv_data = gap_adv_data_new(adv_data_buf, sizeof(adv_data_buf));

  // declarations of variables to be used in this BLE example application
  uint16_t advertising_interval_ms = 1000;
  uint8_t device_name[]            = "TockOS4";

  printf(" - Setting the device name... %s\n", device_name);
  int err = gap_add_device_name(&adv_data, device_name, sizeof(device_name)-1);
  if (err < TOCK_SUCCESS)
    printf("ble_advertise_name, error: %s\r\n", tock_strerror(err));

  // start advertising
  printf(" - Begin advertising! %s\n", device_name);
  err = ble_start_advertising(ADV_NON_CONN_IND, adv_data.buf, adv_data.offset, advertising_interval_ms);
  if (err < TOCK_SUCCESS)
    printf("ble_start_advertising, error: %s\r\n", tock_strerror(err));

  // configuration complete
  printf("Now advertising every %d ms as '%s'\n", advertising_interval_ms,
         device_name);
  return 0;
}
