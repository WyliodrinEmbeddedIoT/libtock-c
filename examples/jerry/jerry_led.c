#include "jerryscript.h"
#include "led.h"
#include <stdlib.h>

static jerry_value_t led_on_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    jerry_value_t ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (led_on (pin));
    }
    return ret_val;
}

static jerry_value_t led_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    return jerry_create_number (led_count ());
}

static jerry_value_t led_off_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    jerry_value_t ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (led_off (pin));
    }
    return ret_val;
}

static jerry_value_t led_toggle_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    jerry_value_t ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (led_toggle (pin));
    }
    return ret_val;
}


jerry_value_t setup_led () 
{
    jerry_value_t led_object = jerry_create_object();
    if (!jerry_value_is_error(led_object))
    {
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "count");
        func_obj = jerry_create_external_function (led_count_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "on");
        func_obj = jerry_create_external_function (led_on_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "off");
        func_obj = jerry_create_external_function (led_off_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "toggle");
        func_obj = jerry_create_external_function (led_toggle_handler);
        jerry_release_value (jerry_set_property (led_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return led_object;
}