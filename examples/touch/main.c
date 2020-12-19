#include <screen.h>
#include <touch.h>
#include <stdlib.h>
#include <timer.h>

static void tocuh_event (int status, int x, int y, void *ud __attribute__ ((unused))) {
  switch (status) {
    case TOUCH_STATUS_PRESSED: {
      printf ("pressed ");
      break;
    }
    case TOUCH_STATUS_RELEASED: {
      printf ("released ");
      break;
    }
    case TOUCH_STATUS_MOVED: {
      printf ("moved ");
      break;
    }
    default:
      printf ("error ");
  }
  printf ("%d x %d y %d\n", status, x, y);
}

int main(void) {
  printf ("Toches: %d\n", get_number_of_touches());
  if (get_number_of_touches () > 0)
  {
    // single_touch_enable ();
    single_touch_set_callback (tocuh_event, NULL);
  }
  else
  {
    printf ("Touch panel is not set up\n");
  }
  return 0;
}
