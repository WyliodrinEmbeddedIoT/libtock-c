#include "date_time.h"
#include "tock.h"

struct data {
  bool fired;
  int year_month_dotm;
  int dotw_hour_min_sec;
};

static struct data result = { .fired = false };

// Internal upcall for faking synchronous reads
static void date_time_upcall(int year_month_dotm,
                             int dotw_hour_min_sec,
                            __attribute__ ((unused)) int unused1,
                            void* ud) {
  struct data* data = (struct data*) ud;
  data->year_month_dotm = year_month_dotm;
  data->dotw_hour_min_sec = dotw_hour_min_sec;
  data->fired    = true;
}

int date_time_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_DATE_TIME, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int date_time_read(void) {
  syscall_return_t rval = command(DRIVER_NUM_DATE_TIME, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int date_time_read_sync(unsigned long* year_month_dotm, unsigned long* dotw_hour_min_sec) {
  int err;
  result.fired = false;

  err = date_time_set_callback(date_time_upcall, (void*) &result);
  if (err < 0) return err;

  err = date_time_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *year_month_dotm= result.year_month_dotm;
  *dotw_hour_min_sec = result.dotw_hour_min_sec;

  return 0;
}


struct date_time get_date_time(void){

    //const unsigned short YEAR_B = 12;
    const unsigned short MONTH_B = 4;
    const unsigned short DAY_B = 5;
    //const unsigned short DOTW_B = 3;
    const unsigned short HOUR_B = 5;
    const unsigned short MIN_B = 6;
    const unsigned short SEC_B = 6;

    unsigned long year_month_dotm;
    unsigned long dotw_hour_min_sec;

    date_time_read_sync(&year_month_dotm, &dotw_hour_min_sec);

    struct date_time dt;

     if (year_month_dotm){
        dt.year = year_month_dotm>>(MONTH_B+DAY_B);
        dt.month = (year_month_dotm<<(32-MONTH_B-DAY_B))>>(32-MONTH_B);
        dt.day = (year_month_dotm<<(32-DAY_B))>>(32-DAY_B);

     }else{
        printf("error whole1 %lu\n ", year_month_dotm);
     }

    if(dotw_hour_min_sec){
        dt.dotw = dotw_hour_min_sec>>(HOUR_B+MIN_B+SEC_B);
        dt.hour = (dotw_hour_min_sec<<(32-HOUR_B-MIN_B-SEC_B))>>(32-HOUR_B);
        dt.minute = ((dotw_hour_min_sec)<<(32-MIN_B-SEC_B))>>(32-MIN_B);
        dt.seconds = ((dotw_hour_min_sec)<<(32-SEC_B))>>(32-SEC_B);

    }else{
        printf("error whole2  %lu\n ", dotw_hour_min_sec);
    }


    return dt;

}