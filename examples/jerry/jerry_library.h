#ifndef JERRY_LIBRARY_H
#define JERRY_LIBRARY_H

#include "jerryscript.h"
#define JERRY_MAX_STRING 500
#define EDGE_RISING  0
#define EDGE_FALLING 1
#define EDGE_BOTH    2
#define EDGE_NONE    3

void jerry_setup ();
void jerry_execute (const char *source);
void jerry_release ();

#endif