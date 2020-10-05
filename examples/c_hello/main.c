/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

char hello[] = "Hello World!\r\n";

static void nop(
  int a __attribute__((unused)),
  int b __attribute__((unused)),
  int c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
  int n = 0;
  while (n < 40) {
    putnstr_async(hello, sizeof(hello), nop, NULL);
    n++;
  }
  return 0;
}
