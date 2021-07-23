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
