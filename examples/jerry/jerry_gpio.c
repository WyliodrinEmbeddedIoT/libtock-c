#include "jerry_gpio.h"

jerry_value_t js_callback_function;

//TODO - callback function dupa ce e gata la button

static jerry_value_t gpio_enable_output_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_enable_output (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    return jerry_create_number (gpio_count());
}

static jerry_value_t gpio_set_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
     if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_set (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_clear_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
     if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_clear (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_toggle_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
     if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_toggle (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_enable_input_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 1)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        GPIO_InputMode_t pin_config = jerry_get_number_value(arguments[1]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_enable_input (pin, pin_config));
    }
    return ret_val;
}

static jerry_value_t gpio_read_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_read (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_enable_interrupt_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 1)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        GPIO_InterruptMode_t irq_config = jerry_get_number_value (arguments[1]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_enable_interrupt (pin, irq_config));
    }
    return ret_val;
}

static jerry_value_t gpio_disable_interrupt_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_disable_interrupt (pin));
    }
    return ret_val;
}

static jerry_value_t gpio_disable_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0)
    {
        GPIO_Pin_t pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (gpio_disable (pin));
    }
    return ret_val;
}

static void interrupt_callback(int pin_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud){
    jerry_value_t args[2] = {pin_num, val};
    jerry_value_t this_val = jerry_create_undefined ();
    jerry_release_value (jerry_call_function (js_callback_function, this_val, args, 2));

    jerry_release_value (this_val);
}

int pin;

static jerry_value_t gpio_interrupt_callback_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0 && jerry_value_is_function (arguments[0]))
    {
        js_callback_function = arguments[0];
        jerry_release_value (ret_val);
        ret_val = jerry_create_number(gpio_interrupt_callback(interrupt_callback, NULL));
    }
    return ret_val;
}



jerry_value_t setup_gpio () 
{
    jerry_value_t gpio_object = jerry_create_object();
    if (!jerry_value_is_error(gpio_object))
    {
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "count");
        func_obj = jerry_create_external_function (gpio_count_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "enable_output");
        func_obj = jerry_create_external_function (gpio_enable_output_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "set");
        func_obj = jerry_create_external_function (gpio_set_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "clear");
        func_obj = jerry_create_external_function (gpio_clear_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "toggle");
        func_obj = jerry_create_external_function (gpio_toggle_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "enable_input");
        func_obj = jerry_create_external_function (gpio_enable_input_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "read");
        func_obj = jerry_create_external_function (gpio_read_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "enable_interrupt");
        func_obj = jerry_create_external_function (gpio_enable_interrupt_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "disable_interrupt");
        func_obj = jerry_create_external_function (gpio_disable_interrupt_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "disable");
        func_obj = jerry_create_external_function (gpio_disable_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "interrupt_callback");
        func_obj = jerry_create_external_function (gpio_interrupt_callback_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        jerry_value_t val;

        prop_name = jerry_create_string ((const jerry_char_t *) "PULL_NONE");
        val = jerry_create_number (PullNone);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);

        prop_name = jerry_create_string ((const jerry_char_t *) "PULL_UP");
        val = jerry_create_number (PullUp);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);

        prop_name = jerry_create_string ((const jerry_char_t *) "PULL_DOWN");
        val = jerry_create_number (PullDown);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);

        prop_name = jerry_create_string ((const jerry_char_t *) "CHANGE");
        val = jerry_create_number (Change);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);

        prop_name = jerry_create_string ((const jerry_char_t *) "RISING_EDGE");
        val = jerry_create_number (RisingEdge);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);

        prop_name = jerry_create_string ((const jerry_char_t *) "FALLING_EDGE");
        val = jerry_create_number (FallingEdge);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, val));
        jerry_release_value (prop_name);
        jerry_release_value (val);
	}
    return gpio_object;
}