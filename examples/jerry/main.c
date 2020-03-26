#include "jerry_library.h"


int main (void)
{
  jerry_setup();

  jerry_execute ("var a = require ('led'); for (var i=0; i<3; i++) a.on(i);");

  jerry_release();
  return 0;
}
