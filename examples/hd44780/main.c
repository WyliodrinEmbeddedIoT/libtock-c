/*
    Example for hd44780 library
    Tested on NUCLEO-F429ZI
 */

#include <hd44780.h>
#include <string.h>
#include <timer.h>
#include <screen.h>
#include <tock.h>

int main(void) {
  int ret = hd44780_init(15);
  ret = hd44780_display();
  if (ret == TOCK_SUCCESS) {
    ret = hd44780_set_cursor(0, 0);
    ret = hd44780_print_string("Hello!");

    delay_ms(5000);

    ret = hd44780_set_cursor(0, 1);
    ret = hd44780_print_string("Goodbyee!");

    delay_ms(2000);
    ret = hd44780_clear();
  }
  return 0;
}