#include "jerry_button.h"

#define MAX_HANDLERS 100

static jerry_value_t button_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    return jerry_create_number (button_count());
}

static jerry_value_t button_enable_interrupt_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (button_enable_interrupt (pin));
    }
    return ret_val;
}

static jerry_value_t button_disable_interrupt_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (button_disable_interrupt (pin));
    }
    return ret_val;
}

static jerry_value_t button_read_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (button_read (pin));
    }
    return ret_val;
}

jerry_value_t js_callback_functions[MAX_HANDLERS];
static int callback_count = 0;

static void button_callback(int pin_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud){
    jerry_value_t args[2];
    args[0] = jerry_create_number (pin_num);
    args[1] = jerry_create_number (val);
    jerry_value_t this_val = jerry_create_undefined ();
    for (int i=0; i<callback_count; i++){
        jerry_release_value (jerry_call_function (js_callback_functions[i], this_val, args, 2));
    }
    jerry_release_value (args[0]);
    jerry_release_value (args[1]);
    jerry_release_value (this_val);
}

static jerry_value_t button_subscribe_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0 && jerry_value_is_function (arguments[0]))
    {
        jerry_value_t js_callback_function = jerry_acquire_value(arguments[0]);
        js_callback_functions[callback_count++] = js_callback_function;
        jerry_release_value (ret_val);
        jerry_release_value (js_callback_function);
        ret_val = jerry_create_number(button_subscribe(button_callback, NULL));
    }
    return ret_val;
}

jerry_value_t setup_button () 
{
    jerry_value_t button_object = jerry_create_object();
    if (!jerry_value_is_error(button_object))
    {
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "count");
        func_obj = jerry_create_external_function (button_count_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "enable_interrupt");
        func_obj = jerry_create_external_function (button_enable_interrupt_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "disable_interrupt");
        func_obj = jerry_create_external_function (button_disable_interrupt_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "read");
        func_obj = jerry_create_external_function (button_read_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "subscribe");
        func_obj = jerry_create_external_function (button_subscribe_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return button_object;
}