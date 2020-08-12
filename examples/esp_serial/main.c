/* vim: set sw=2 expandtab tw=80: */
// bind -> port (if 0 -> random)
// listen -> port
// send
// recv
// close
// socket -> first available socket

#include <stdio.h>
#include <esp_serial.h>
#include <timer.h>

static void esp_event (char* buffer, int len) {
    printf("am primit buffer in userspace de lungime %d - %s\n", len, buffer);
}

int main(void) {
  subscribe_user_callback(esp_event, NULL);
  int portSrc = 2399;
  int linkId;
  bind("192.168.1.100", 8080, &portSrc, &linkId);
  // send ip, port, mesaj, lungime
  send_UDP_payload(18, "salut, ce mai faci", linkId);
  send_UDP_payload(27, "ce parere ai daca ai merge?", linkId);
  // demo - hardcoded ip (esp in not connected yet)
  get_esp_ip();
  return 0;
}
