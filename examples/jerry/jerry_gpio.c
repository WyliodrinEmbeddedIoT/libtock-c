#include "jerryscript.h"
#include "gpio.h"

//int gpio_enable_output(int pin);
// int gpio_set(int pin);
// int gpio_clear(int pin);
// int gpio_toggle(int pin);
// int gpio_enable_input(GPIO_Pin_t pin, GPIO_InputMode_t pin_config);
// int gpio_read(GPIO_Pin_t pin);
// int gpio_enable_interrupt(GPIO_Pin_t pin, GPIO_InterruptMode_t irq_config);
// int gpio_disable_interrupt(GPIO_Pin_t pin);
// int gpio_disable(GPIO_Pin_t pin);
// int gpio_interrupt_callback(subscribe_cb callback, void* callback_args);

static int gpio_enable_output_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        return gpio_enable_output (pin);
    }
    return jerry_create_undefined ();
}

static int gpio_toggle_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count)
{
    if (arguments_count > 0)
    {
        int pin = jerry_get_number_value(arguments[0]);
        return gpio_toggle (pin); 
    }
    return jerry_create_undefined ();
}

jerry_value_t setup_gpio () 
{
    jerry_value_t gpio_object = jerry_create_object();
    if (!jerry_value_is_error(gpio_object))
    {
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "enable_output");
        func_obj = jerry_create_external_function (gpio_enable_output_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "toggle");
        func_obj = jerry_create_external_function (gpio_toggle_handler);
        jerry_release_value (jerry_set_property (gpio_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return gpio_object;
}