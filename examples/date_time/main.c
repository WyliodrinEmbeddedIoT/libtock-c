#include <stdbool.h>
#include <stdio.h>

//#include <humidity.h>

#include <timer.h>
#include <tock.h>
#include <date_time.h>


static tock_timer_t timer;

//static bool humidity       = false;
static bool date_time = false;


void test_time(){
    unsigned long year_month_dotm= 0;
      unsigned long dotw_hour_min_sec= 0;




      if (date_time)       date_time_read_sync(&year_month_dotm, &dotw_hour_min_sec);



     printf("whole1 %lu%%\n ", year_month_dotm);
      if (year_month_dotm){
            unsigned long year = year_month_dotm>>(4+5);
            unsigned long month = (year_month_dotm<<(32-5-4))>>(32-5-4);
            unsigned long dotm = (year_month_dotm<<(32-5))>>(32-5);
            printf("whole1 %lu%%\n ", year_month_dotm);
            printf("YEAR:                    %lu%%\n", year);
            printf("MONTH:                    %lu%%\n", month);
            printf("DOTM:                    %lu%%\n", dotm);
        }else{
            printf("error whole1 %lu%%\n ", year_month_dotm);
        }

        printf("whole2  %lu%%\n ", dotw_hour_min_sec);
        if(dotw_hour_min_sec){
            unsigned long dotw = dotw_hour_min_sec>>(5+6+6);
            unsigned long hour = (dotw_hour_min_sec<<(32-5-6-6))>>(32-5-6-6);
            unsigned long min = ((dotw_hour_min_sec)<<(32-5-6))>>(32-5-6);
            unsigned long sec = ((dotw_hour_min_sec)<<(32-5))>>(32-5);
            printf("whole2  %lu%%\n ", dotw_hour_min_sec);
            printf("DOTW:                    %lu%%\n", dotw);
            printf("HOUR:                    %lu%%\n", hour);
            printf("MIN:                    %lu%%\n", min);
            printf("SEC:                    %lu%%\n", sec);
        }else{
            printf("error whole2  %lu%%\n ", dotw_hour_min_sec);
        }



      printf("\n");



}

static void timer_fired(__attribute__ ((unused)) int arg0,
                        __attribute__ ((unused)) int arg1,
                        __attribute__ ((unused)) int arg2,
                        __attribute__ ((unused)) void* ud) {


}

int main(void) {
    date_time       = driver_exists(DRIVER_NUM_DATE_TIME);

    while(true){
         test_time();
         delay_ms(1000);

    }





  //timer_in(1000, timer_fired, NULL, &timer);

  return 0;
}
