#include "jerry_library.h"


int main (void)
{
  jerry_setup();

  jerry_execute ("console.error ('ioana_error'); console.log ('ioana');");

  jerry_release();
  return 0;
}
