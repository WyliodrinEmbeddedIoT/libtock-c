#include "jerry_button.h"

typedef struct {
  jerry_value_t cb_function;
  int edge;
} button_cb_t;

static jerry_value_t button_count_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    return jerry_create_number (button_count());
}

static void button_callback(int pin_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud){
    jerry_value_t fct = ((button_cb_t *)ud)->cb_function;
    int edge = ((button_cb_t *)ud)->edge;
    if ((edge == EDGE_RISING && val == 1) || (edge == EDGE_FALLING && val == 0) || (edge == EDGE_BOTH)){
        jerry_value_t args[1];
        args[0] = jerry_create_number (val);
        jerry_value_t this_val = jerry_create_undefined ();
        jerry_release_value (jerry_call_function (fct, this_val, args, 1));
        jerry_release_value (args[0]);
        jerry_release_value (this_val);
    }
}

static jerry_value_t button_watch_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    if (arguments_count > 0 && jerry_value_is_function (arguments[0])){
        jerry_value_t js_callback_function = jerry_acquire_value(arguments[0]);

        jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "edge");
        jerry_value_t prop_value = jerry_get_property(function_this, prop_name);
        int edge = jerry_get_number_value (prop_value);
        jerry_release_value (prop_name);
        jerry_release_value (prop_value);

        prop_name = jerry_create_string ((const jerry_char_t *) "pin");
        prop_value = jerry_get_property(function_this, prop_name);
        int pin = jerry_get_number_value (prop_value);
        jerry_release_value (prop_name);
        jerry_release_value (prop_value);

        int ret_val = button_enable_interrupt (pin);
        if (ret_val == TOCK_SUCCESS){
            button_cb_t *ud = malloc (sizeof(button_cb_t));
            ud->cb_function = js_callback_function;
            ud->edge = edge;
            button_subscribe(button_callback, ud);
        }
    }
    return jerry_create_undefined();
}

static jerry_value_t button_read_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int ret_val = jerry_create_number (TOCK_EINVAL);
    if (arguments_count > 0){
        int pin = jerry_get_number_value(arguments[0]);
        jerry_release_value (ret_val);
        ret_val = jerry_create_number (button_read (pin));
    }
    return ret_val;
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

            if (arguments_count == 2){
                prop_name = jerry_create_string ((const jerry_char_t *) "edge");
                jerry_release_value (jerry_set_property (function_this, prop_name, arguments[1]));
                jerry_release_value (prop_name);
            }
            else{
                prop_name = jerry_create_string ((const jerry_char_t *) "edge");
                jerry_value_t edge = jerry_create_number (EDGE_BOTH);
                jerry_release_value (jerry_set_property (function_this, prop_name, edge));
                jerry_release_value (prop_name);
                jerry_release_value (edge);
            }
            prop_name = jerry_create_string ((const jerry_char_t *) "watch");
            jerry_value_t func_obj = jerry_create_external_function (button_watch_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);
        }
    }
    jerry_release_value (new_target);

  return jerry_create_undefined ();
}

jerry_value_t setup_button () {
    jerry_value_t button_object = jerry_create_object();
    if (!jerry_value_is_error(button_object)){
        jerry_value_t prop_name;
        jerry_value_t func_obj;

        prop_name = jerry_create_string ((const jerry_char_t *) "count");
        func_obj = jerry_create_external_function (button_count_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "Button");
        func_obj = jerry_create_external_function (new_button_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "read");
        func_obj = jerry_create_external_function (button_read_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "EDGE_RISING");
        jerry_value_t const_obj = jerry_create_number (EDGE_RISING);
        jerry_release_value (jerry_set_property (button_object, prop_name, const_obj));
        jerry_release_value (prop_name);
        jerry_release_value (const_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "EDGE_FALLING");
        const_obj = jerry_create_number (EDGE_FALLING);
        jerry_release_value (jerry_set_property (button_object, prop_name, const_obj));
        jerry_release_value (prop_name);
        jerry_release_value (const_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "EDGE_BOTH");
        const_obj = jerry_create_number (EDGE_BOTH);
        jerry_release_value (jerry_set_property (button_object, prop_name, const_obj));
        jerry_release_value (prop_name);
        jerry_release_value (const_obj);
	}
    return button_object;
}