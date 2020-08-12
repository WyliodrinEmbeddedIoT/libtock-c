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
  // connect_to_wifi("salut", "parola");
  subscribe_user_callback(esp_event, NULL);
  int portSrc = 2399;
  int linkId;
  bind("192.168.1.100", 8080, &portSrc, &linkId);
  // delay_ms(1000);
  // send ip, port, mesaj, lungime
  // esp send_to
  send_UDP_payload(18, "salut, ce mai faci", linkId);
  // delay_ms(1000);
  // fake_receive();
  // close_socket();
  
  return 0;
}
