/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <esp_serial.h>


int main(void) {
  int ret = bind_socket(192, 168, 1, 1, 8080);
  return 0;
}
