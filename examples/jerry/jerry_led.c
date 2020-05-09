#include "jerry_led.h"

static int get_pin (jerry_value_t jerry_obj){
    jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "pin");
    jerry_value_t prop_value = jerry_get_property(jerry_obj, prop_name);
    int pin = jerry_get_number_value (prop_value);
    jerry_release_value (prop_name);
    jerry_release_value (prop_value);
    return pin;
}

// static jerry_value_t led_on_handler (const jerry_value_t function_object,
//                const jerry_value_t function_this,
//                const jerry_value_t arguments[],
//                const jerry_length_t arguments_count){
//     int pin = get_pin (function_this);
//     return jerry_create_number (led_on (pin));
// }

static jerry_value_t led_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    return jerry_create_number (led_count ());
}

static jerry_value_t led_write_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t ret_val = jerry_create_undefined ();
    if (arguments_count > 0){
        int pin = get_pin (function_this);
        int val = jerry_get_number_value (arguments[0]);
        jerry_release_value (ret_val);
        if (val == 1) ret_val = jerry_create_number (led_on (pin));
        else ret_val = jerry_create_number (led_on (pin));
    }
    return ret_val;
}

static jerry_value_t led_toggle_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    return jerry_create_number (led_toggle (pin));
}

static jerry_value_t new_led_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t new_target = jerry_get_new_target ();

    if (jerry_value_get_type (new_target) == JERRY_TYPE_FUNCTION){
        if (arguments_count > 0){
            jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "pin");
            jerry_release_value (jerry_set_property (function_this, prop_name, arguments[0]));
            jerry_release_value (prop_name);

            if (arguments_count > 1){
                jerry_value_t mode_value = jerry_value_to_string (arguments[1]);
                jerry_size_t string_size = jerry_get_string_size (mode_value    );
    
                char *mode_val = (char*) malloc (string_size + 1);

                if (mode_val!=NULL){
                    int n = jerry_substring_to_char_buffer(mode_value, 0, string_size, (jerry_char_t*)mode_val, string_size);
                    mode_val[n] = '\0';

                    int pin = get_pin (function_this);
                    if (strncmp (mode_val, "high", JERRY_MAX_STRING) == 0) led_on (pin);
                    else if (strncmp (mode_val, "falling", JERRY_MAX_STRING) == 0) led_off (pin);

                    free (mode_val);
                }
                jerry_release_value (mode_value);
                jerry_release_value (string_size);
            }

            prop_name = jerry_create_string ((const jerry_char_t *) "writeSync");
            jerry_value_t func_obj = jerry_create_external_function (led_write_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "toggle");
            func_obj = jerry_create_external_function (led_toggle_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);
        }
    }
    jerry_release_value (new_target);

    return jerry_create_undefined ();
}

jerry_value_t setup_led () {
    jerry_value_t led_object = jerry_create_object();
    if (!jerry_value_is_error(led_object)){
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "count");
        func_obj = jerry_create_external_function (led_count_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "Led");
        func_obj = jerry_create_external_function (new_led_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return led_object;
}