#include "jerry_library.h"


int main (void)
{
  jerry_setup();

  jerry_execute ("var led = require ('led'); for (var i=0; i<led.count(); i=i+1) {led.toggle(i);sleep();led.toggle (i);};");

  jerry_release();
  return 0;
}
