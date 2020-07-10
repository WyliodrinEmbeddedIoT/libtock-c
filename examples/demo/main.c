/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <timer.h>
#include <led.h>
#include <ninedof.h>

#include <console.h>

static bool ninedof = false;

int f() {
  printf("text\r\n");
  f();
}

int main(void) {
  printf("Demo Embedded Linux Conference! O_O ^_^\r\n");
  char c = 0;
  char str[10];
  int i = 0;
  printf("What is your name?\r\n");
  c = getch();
  while (c != 13) {
    str[i++] = c;
    c = getch();
  }

  printf("Welcome %s!\r\n"
    "Type \"o\" to turn led on, or \"f\" to turn led off!\r\n"
    "Type \"s\" to print accelerometer data!\r\n"
    "Type \"x\" to fault!\r\n"
    "Type \"y\" to stack overflow!\r\n"
    "Press \"e\" to exit!\r\n", str);
  while (1)
  {
    c = getch();
    printf("You typed: %c!\r\n", c);
    if (c == 'o')
      led_on(0);
    else if (c == 'f')
      led_off(0);
    else if (c == 's') {
      ninedof     = driver_exists(DRIVER_NUM_NINEDOF);

      int ninedof_x = 0, ninedof_y = 0, ninedof_z = 0;
      if (ninedof)      ninedof_read_magnetometer_sync(&ninedof_x, &ninedof_y, &ninedof_z);
      if (ninedof)        printf("FXOS8700CQ: X:               %d\r\n", ninedof_x);
      if (ninedof)        printf("FXOS8700CQ: Y:               %d\r\n", ninedof_y);
      if (ninedof)        printf("FXOS8700CQ: Z:               %d\r\n", ninedof_z);
    }
    else if (c == 'e')
      break;
    else if (c == 'x') {
      char *p = NULL;
      *p = 2;
    }
    else if (c == 'y') {
      f();
    }
  }
  printf("Good\r\n");
  delay_ms(1000);

  printf("Bye!\r\n");
  delay_ms(2000);

  printf("^^ ^^ ^_^\r\n");
  delay_ms(3000);

  printf("^^ ^^ ^_^\r\n");
  delay_ms(4000);

  printf("^^ ^^ ^_^\r\n");
  delay_ms(5000);

  printf("^^ ^^ ^_^\r\n");
  return 0;
}