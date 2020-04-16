#include "jerry_led.h"

static int get_pin (jerry_value_t jerry_obj){
    jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "pin");
    jerry_value_t prop_value = jerry_get_property(jerry_obj, prop_name);
    int pin = jerry_get_number_value (prop_value);
    jerry_release_value (prop_name);
    jerry_release_value (prop_value);
    return pin;
}

static jerry_value_t led_on_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    jerry_release_value (ret_val);
    return jerry_create_number (led_on (pin));
}

static jerry_value_t led_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    return jerry_create_number (led_count ());
}

static jerry_value_t led_off_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    jerry_release_value (ret_val);
    return jerry_create_number (led_off (pin));
}

static jerry_value_t led_toggle_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    jerry_release_value (ret_val);
    return jerry_create_number (led_toggle (pin));
}

static jerry_value_t new_button_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t new_target = jerry_get_new_target ();

    if (jerry_value_get_type (new_target) == JERRY_TYPE_FUNCTION){
        if (arguments_count > 1){
            jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "pin");
            jerry_release_value (jerry_set_property (function_this, prop_name, arguments[0]));
            jerry_release_value (prop_name);

            prop_name = jerry_create_string ((const jerry_char_t *) "on");
            jerry_value_t func_obj = jerry_create_external_function (led_on_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "off");
            func_obj = jerry_create_external_function (led_off_handler);
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
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return led_object;
}