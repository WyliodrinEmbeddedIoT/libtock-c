#include "led_rgb.h"


int led_found(void) {
  syscall_return_t rval = command(DRIVER_NUM_LED_RGB, 0, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}


int led_rgb_on(int r, int g, int b) {
   unsigned int rgb = r;
   rgb = (rgb<<8) + g;
   rgb = (rgb<<8) + b;
  syscall_return_t rval = command(DRIVER_NUM_LED_RGB, 1, rgb, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int led_rgb_off(void) {
  syscall_return_t rval = command(DRIVER_NUM_LED_RGB, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}