#include <digit.h>
#include <timer.h>

int main(void) {
  // Ask the kernel how many digits are on this board.
  int num_digits;
  int err = digit_count(&num_digits);
  if (err < 0) return err;

  print_digit(1, 1);
  delay_ms(1000);
  clear_digits();
  print_digit(2, 2);
  delay_ms(1000);
  clear_digits();
  print_digit(3, 3);
  delay_ms(1000);
  clear_digits();
  print_digit(4, 4);
  delay_ms(1000);
  clear_digits();
  print_digit(1, 2);
  print_digit(2, 0);
  print_digit(3, 2);
  print_digit(4, 2);
  delay_ms(1000);
  print_dot(1);
  delay_ms(1000);
  print_dot(2);
  delay_ms(1000);
  print_dot(3);
  delay_ms(1000);
  print_dot(4);
}
