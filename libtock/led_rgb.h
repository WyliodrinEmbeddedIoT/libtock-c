#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LED_RGB 0x00090004

int led_found(void);
int led_rgb_on(int r, int g, int b);
int led_rgb_off(void);

#ifdef __cplusplus
}
#endif
