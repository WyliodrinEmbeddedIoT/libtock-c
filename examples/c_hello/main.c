/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ipc.h>
#include <timer.h>

#include <console.h>

char seg_buf[64] __attribute__((aligned(64)));

char hello[] = "Hello World!\r\n";

static void nop(
  int a __attribute__((unused)),
  int b __attribute__((unused)),
  int c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
  putnstr_async(hello, sizeof(hello), nop, NULL);

  int seg_service = ipc_discover("7seg");
  // ipc_register_client_cb(seg_service, ipc_callback, NULL);
  ipc_share(seg_service, seg_buf, 64);

  for (int i=0; i<9999; i++) {
    seg_buf[0] = (i >> 8) & 0xFF;
    seg_buf[1] = i & 0xFF;
    printf ("i %d\n", seg_buf[1]);
    ipc_notify_svc(seg_service);
    delay_ms (1000);
  }

  return 0;
}
