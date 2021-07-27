#include <stdbool.h>
#include <stdio.h>

#include <timer.h>
#include <tock.h>
#include <date_time.h>


static bool date_time = false;

void test_time(void);

int main(void) {

    printf("testing rtc date time");
    date_time = driver_exists(DRIVER_NUM_DATE_TIME);

    while(true){
         test_time();
         delay_ms(1000);
    }

  return 0;
}



void test_time(void){

    struct date_time dt;

      if (date_time) {
        dt = get_date_time();

        printf("YEAR:%u\n", dt.year);
        printf("MONTH:%u\n", dt.month);
        printf("DOTM: %u\n", dt.day);

        printf("DOTW: %u\n", dt.dotw);
        printf("HOUR:  %u\n", dt.hour);
        printf("MIN: %u\n", dt.minute);
        printf("SEC:  %u\n", dt.seconds);
      }

      printf("\n");

}



