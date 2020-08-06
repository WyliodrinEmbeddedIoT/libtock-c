/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <esp_serial.h>
#include <timer.h>

int main(void) {
  int ret = bind_socket(192, 168, 1, 1, 8080);
  printf("\r\nfirst command\r\n");
  delay_ms(1000);
  send_UDP_payload(18, "salut, ce mai faci");
  printf("\r\n2nd command\r\n");
  delay_ms(1000);
  close_socket();
  return 0;
}
