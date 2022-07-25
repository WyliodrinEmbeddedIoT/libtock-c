#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_DIGITS 0x90004

int digit_count(int* count);

int print_digit(int position, int digit);

int clear_digits(void);

int print_dot(int position);

int print_pattern(int position, int pattern);

#ifdef __cplusplus
}
#endif