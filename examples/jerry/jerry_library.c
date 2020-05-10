#include "jerryscript.h"
#include "jerry_library.h"
#include "jerry_console.h"
#include "jerry_gpio.h"
#include "jerry_led.h"
#include <string.h>
#include <stdlib.h>
#include <timer.h>
typedef struct {
  tock_timer_t tock_timer;
  jerry_value_t timeout_function;
  bool call_free;
} user_data_t;

double jerry_port_get_current_time (void){
    return 0;
}

double jerry_port_get_local_time_zone_adjustment (double unix_ms, bool is_utc){
    return 0;
}

void JERRY_ATTR_NORETURN jerry_port_fatal (jerry_fatal_code_t code){}

size_t jerry_port_normalize_path (const char *in_path_p,
                                  char *out_buf_p,
                                  size_t out_buf_size,
                                  char *base_file_p){
    return (size_t) 0;
}

jerry_value_t jerry_port_get_native_module (jerry_value_t name){
    return jerry_create_undefined();
}

uint8_t *jerry_port_read_source (const char *file_name_p, size_t *out_size_p){
    return NULL;
}

void jerry_port_release_source (uint8_t *buffer_p){
}

static jerry_value_t require_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t ret_val = jerry_create_undefined();
    if (arguments_count > 0)
    {
        char *library_name = NULL;
        jerry_value_t string_value = jerry_value_to_string (arguments[0]);
        jerry_size_t string_size = jerry_get_string_size (arguments[0]);
        library_name = (char*) malloc (string_size + 1);

        if (library_name!=NULL)
        {
            int n = jerry_substring_to_char_buffer(string_value, 0, string_size, (jerry_char_t*)library_name, string_size);
            library_name[n] = '\0';

            if (strncmp (library_name, "onoff", JERRY_MAX_STRING) == 0)
            {
                jerry_value_t onoff_object = jerry_create_object();
                if (!jerry_value_is_error(onoff_object)){
                    jerry_value_t prop_name;
                    jerry_value_t prop_value;
                    
                    prop_name = jerry_create_string ((const jerry_char_t *) "Button");
                    prop_value = setup_button ();
                    jerry_release_value (jerry_set_property (onoff_object, prop_name, prop_value));
                    jerry_release_value (prop_name);
                    jerry_release_value (prop_value);

                    prop_name = jerry_create_string ((const jerry_char_t *) "Led");
                    prop_value = setup_led ();
                    jerry_release_value (jerry_set_property (onoff_object, prop_name, prop_value));
                    jerry_release_value (prop_name);
                    jerry_release_value (prop_value);

                    prop_name = jerry_create_string ((const jerry_char_t *) "Led");
                    prop_value = setup_led ();
                    jerry_release_value (jerry_set_property (onoff_object, prop_name, prop_value));
                    jerry_release_value (prop_name);
                    jerry_release_value (prop_value);

                    prop_name = jerry_create_string ((const jerry_char_t *) "HIGH");
                    prop_value = jerry_create_number (1);
                    jerry_release_value (jerry_set_property (onoff_object, prop_name, prop_value));
                    jerry_release_value (prop_name);
                    jerry_release_value (prop_value);

                    prop_name = jerry_create_string ((const jerry_char_t *) "LOW");
                    prop_value = jerry_create_number (0);
                    jerry_release_value (jerry_set_property (onoff_object, prop_name, prop_value));
                    jerry_release_value (prop_name);
                    jerry_release_value (prop_value);
                }
                ret_val = onoff_object;
            }
            // if (strncmp (library_name, "gpio", JERRY_MAX_STRING) == 0)
            // {
            //     jerry_release_value (ret_val);
            //     ret_val = setup_gpio ();
            // }
            // else if (strncmp (library_name, "led", JERRY_MAX_STRING) == 0){
            //     jerry_release_value (ret_val);
            //     ret_val = setup_led ();
            // }
            // else if (strncmp (library_name, "button", JERRY_MAX_STRING) == 0){
            //     jerry_release_value (ret_val);
            //     ret_val = setup_button ();
            // }

            free(library_name);
        }
        jerry_release_value(string_value);
    }
    return ret_val;
}

static void timeout_callback(__attribute__ ((unused)) int now,
                          __attribute__ ((unused)) int expiration,
                          __attribute__ ((unused)) int unused,
                          void* ud){
    jerry_value_t fct = ((user_data_t *)ud)->timeout_function;
    jerry_value_t this_val = jerry_create_undefined ();
    
    jerry_release_value (jerry_call_function (fct, this_val, NULL, 0));

    jerry_release_value (this_val);

    bool call_free = ((user_data_t *)ud)->call_free;
    if (call_free){ 
        jerry_release_value (fct);
        free (ud);
    }
}

static jerry_value_t timeout_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t ret_val = jerry_create_undefined();
    if (arguments_count > 1 && jerry_value_is_function (arguments[0])){
        jerry_value_t js_callback_function = jerry_acquire_value(arguments[0]);
        jerry_value_t ms = jerry_get_number_value (arguments[1]);

        user_data_t *ud = malloc (sizeof(user_data_t));
        ud->timeout_function = js_callback_function;
        ud->call_free = false;
        timer_in (ms, timeout_callback, ud, &(ud->tock_timer));
        jerry_release_value (ms);
    }
    return ret_val;
}

static jerry_value_t interval_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t ret_val = jerry_create_undefined();
    if (arguments_count > 1 && jerry_value_is_function (arguments[0])){
        jerry_value_t js_callback_function = jerry_acquire_value(arguments[0]);
        jerry_value_t ms = jerry_get_number_value (arguments[1]);

        user_data_t *ud = malloc (sizeof(user_data_t));
        ud->timeout_function = js_callback_function;
        ud->call_free = false;
        timer_every (ms, timeout_callback, ud, &(ud->tock_timer));
        jerry_release_value (ms);
    }
    return ret_val;
}


void jerry_setup(){
    jerry_init(JERRY_INIT_EMPTY);

	jerry_value_t global_object = jerry_get_global_object ();

    if (!jerry_value_is_error(global_object))
    {
        jerry_value_t property_name_require = jerry_create_string ((const jerry_char_t *) "require");
        jerry_value_t property_value_func = jerry_create_external_function (require_handler);
        jerry_release_value (jerry_set_property (global_object, property_name_require, property_value_func));
        jerry_release_value (property_name_require);
        jerry_release_value (property_value_func);

        property_name_require = jerry_create_string ((const jerry_char_t *) "setTimeout");
        property_value_func = jerry_create_external_function (timeout_handler);
        jerry_release_value (jerry_set_property (global_object, property_name_require, property_value_func));
        jerry_release_value (property_name_require);
        jerry_release_value (property_value_func);

        property_name_require = jerry_create_string ((const jerry_char_t *) "setInterval");
        property_value_func = jerry_create_external_function (interval_handler);
        jerry_release_value (jerry_set_property (global_object, property_name_require, property_value_func));
        jerry_release_value (property_name_require);
        jerry_release_value (property_value_func);

        jerry_value_t property_name_console = jerry_create_string ((const jerry_char_t *) "console");
        jerry_value_t console = setup_console();
        jerry_release_value (jerry_set_property (global_object, property_name_console, console));
        jerry_release_value (property_name_console);
        jerry_release_value (console);

    }
	jerry_release_value (global_object);
}

void jerry_execute (const char *source){
    jerry_value_t parsed_code = jerry_parse (NULL, 0, source, strlen (source) - 1, JERRY_PARSE_NO_OPTS);

    if (!jerry_value_is_error (parsed_code))
    {
        jerry_release_value (jerry_run (parsed_code));
    }
    else
    {
        printf ("parse error\n");
    }
    jerry_release_value (parsed_code);
}

void jerry_release (){
    jerry_cleanup();
}
