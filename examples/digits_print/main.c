#include <digit.h>
#include <timer.h>

int main(void) {
  // Ask the kernel how many digits are on this board.
  int num_digits;
  int err = digit_count(&num_digits);
  if (err < 0) return err;

  // 1
  print_digit(1, 1);
  delay_ms(1000);
  // 2
  clear_digits();
  print_digit(2, 2);
  delay_ms(1000);
  // 3
  clear_digits();
  print_digit(3, 3);
  delay_ms(1000);
  // 4
  clear_digits();
  print_digit(4, 4);
  delay_ms(1000);
  // 2022
  clear_digits();
  print_digit(1, 2);
  print_digit(2, 0);
  print_digit(3, 2);
  print_digit(4, 2);
  delay_ms(1000);
  // 2.0.2.2.
  print_dot(1);
  delay_ms(1000);
  print_dot(2);
  delay_ms(1000);
  print_dot(3);
  delay_ms(1000);
  print_dot(4);
  delay_ms(1000);
  // ALO.
  print_pattern(1, 0b01110111);
  print_pattern(2, 0b00111000);
  print_pattern(3, 0b00111111);
  print_pattern(4, 0b10000000);
}
