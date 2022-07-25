#include "digit.h"

int digit_count(int* count) {
    syscall_return_t rval = command(DRIVER_NUM_DIGITS, 0, 0, 0);
    return tock_command_return_u32_to_returncode(rval, (uint32_t*) count);
}

int print_digit(int position, int digit) {
    syscall_return_t rval = command(DRIVER_NUM_DIGITS, 1, position, digit);
    return tock_command_return_novalue_to_returncode(rval);
}

int clear_digits(void) {
    syscall_return_t rval = command(DRIVER_NUM_DIGITS, 2, 0, 0);
    return tock_command_return_novalue_to_returncode(rval);
}

int print_dot(int position) {
    syscall_return_t rval = command(DRIVER_NUM_DIGITS, 3, position, 0);
    return tock_command_return_novalue_to_returncode(rval);
}

int print_pattern(int position, int pattern) {
    syscall_return_t rval = command(DRIVER_NUM_DIGITS, 4, position, pattern);
    return tock_command_return_novalue_to_returncode(rval);
}