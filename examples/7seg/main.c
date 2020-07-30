/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gpio.h>
#include <timer.h>
#include <ipc.h>

const unsigned char SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const unsigned char SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

int n = 0;

void shift_out (GPIO_Pin_t data, GPIO_Pin_t clock, unsigned char value);
void set_number (int number, int segment);

void shift_out (GPIO_Pin_t data, GPIO_Pin_t clock, unsigned char value) {
  gpio_enable_output (data);
  gpio_enable_output (clock);
  for (int i = 7; i >= 0; i--) {
    if (((value >> i) & 0x01) == 1) {
      gpio_set (data);
    }
    else
    {
      gpio_clear (data);
    }
    gpio_set (clock);
    gpio_clear (clock);
  }
}

void set_number (int number, int segment) {
  gpio_clear (4);
  shift_out (8, 7, SEGMENT_MAP[number]);
  shift_out (8, 7, SEGMENT_SELECT[segment]);
  gpio_set (4);
}

static void ipc_callback(int pid, int len, int buf, void* ud) {
  // pid: An identifier for the app that notified us.
  // len: How long the buffer is that the client shared with us.
  // buf: Pointer to the shared buffer.
  uint8_t* buffer = (uint8_t*) buf;
  if (len >= 2) {
    n = (buffer[0] << 8) + buffer[1];
  }
  printf ("number %d\n", n);
  ipc_notify_client(pid);
}

int main(void) {
  gpio_enable_output (4);
  ipc_register_svc(ipc_callback, NULL);
  for (;;)
  {
      set_number (n%10, 3);
      if (n >= 10) set_number (n/10%10, 2);
      if (n >= 100) set_number (n/100%10, 1);
      if (n >= 1000) set_number (n/1000%10, 0);
      delay_ms (3);
  }
  return 0;
}
