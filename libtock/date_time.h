#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_DATE_TIME 0x90004


struct date_time{
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned dotw;
    unsigned hour;
    unsigned minute;
    unsigned seconds;
};

// function to be called when the date timer reading is finished
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int date_time_set_callback (subscribe_upcall callback, void* callback_args);

// initiate an date time reading used both for syncronous and asyncronous readings
int date_time_read(void);

struct date_time get_date_time(void);
// initiate a syncronous date time reading
//
// year_month_dotm           - pointer/address where the result of the date_time reading should be stored
int date_time_read_sync (unsigned long* year_month_dotm, unsigned long* dotw_hour_min_sec);

#ifdef __cplusplus
}
#endif
