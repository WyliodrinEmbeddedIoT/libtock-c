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

int main(void) {
  // connect_to_wifi("salut", "parola");
  int portSrc = 8080;
  int linkId;
  int ret = bind("127.0.0.1", 2399, &portSrc, &linkId);
  // delay_ms(1000);
  // send ip, port, mesaj, lungime
  // esp send_to
  send_UDP_payload(18, "salut, ce mai faci", linkId);
  // delay_ms(1000);
  // fake_receive();
  // close_socket();
  
  return 0;
}
