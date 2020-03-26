#ifndef JERRY_LIBRARY_H
#define JERRY_LIBRARY_H

#include "jerryscript.h"
#define JERRY_MAX_STRING 500

void jerry_setup ();
void jerry_execute (const char *source);
void jerry_release ();

#endif