#include "lvgl_driver.h"
#include <stdio.h>
#include <timer.h>

int main (void)
{
  screen_set_brightness (100);
  int status = lvgl_driver_init (5);
  if (status == TOCK_SUCCESS) {
    /* LittlevGL's Hello World tutorial example */

    lv_obj_t * scr = lv_disp_get_scr_act(NULL);         /*Get the current screen*/

    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(scr, NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Loading ...");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);


    /*Describe the color for the needles*/
    static lv_color_t needle_colors[3];
    needle_colors[0] = LV_COLOR_BLUE;
    // needle_colors[1] = LV_COLOR_ORANGE;
    // needle_colors[2] = LV_COLOR_PURPLE;

    /*Create a gauge*/
    lv_obj_t * gauge1 = lv_gauge_create(lv_scr_act(), NULL);
    lv_gauge_set_needle_count(gauge1, 1, needle_colors);
    lv_obj_set_size(gauge1, 100, 100);
    lv_obj_align(gauge1, NULL, LV_ALIGN_CENTER, 0, 0);

    /*Set the values*/
    lv_gauge_set_value(gauge1, 0, 0);

    int i = 0;

    /* main loop */
    while (1)
    {
      delay_ms (5);
      i=i+1;
      if (i == 1)
      {
        lv_label_set_text(label1, "Loading ...");
        lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
      }
      else
      if (i < 100)
      {
        lv_gauge_set_value(gauge1, 0, i);
      }
      else if (i == 100)
      {
        lv_label_set_text(label1, "Hello TockOS");
        lv_obj_align(label1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
      }
      if (i > 500) i = 0;
      lvgl_driver_event (5);
    }
  } else {
    printf ("lvgl init error: %s\n", tock_strerror(status));
  }
  return 0;
}

