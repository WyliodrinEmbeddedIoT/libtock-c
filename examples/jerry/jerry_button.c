#include "jerry_button.h"

typedef struct {
  jerry_value_t cb_function;
  jerry_value_t edge;
  int pin;

} button_cb_t;

static int get_pin (jerry_value_t jerry_obj){
    jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "_pin");
    jerry_value_t prop_value = jerry_get_property(jerry_obj, prop_name);
    int pin = jerry_get_number_value (prop_value);
    jerry_release_value (prop_name);
    jerry_release_value (prop_value);
    return pin;
}

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
    jerry_value_t edge_string = ((button_cb_t *)ud)->edge;
    

    jerry_value_t string_value = jerry_value_to_string (edge_string);
    jerry_size_t string_size = jerry_get_string_size (edge_string);
    
    char *edge_val = (char*) malloc (string_size + 1);
    int edge = EDGE_NONE;

    if (edge_val!=NULL){
        int n = jerry_substring_to_char_buffer(string_value, 0, string_size, (jerry_char_t*)edge_val, string_size);
        edge_val[n] = '\0';

        if (strncmp (edge_val, "rising", JERRY_MAX_STRING) == 0) edge = EDGE_RISING;
        else if (strncmp (edge_val, "falling", JERRY_MAX_STRING) == 0) edge = EDGE_FALLING;
        else if (strncmp (edge_val, "both", JERRY_MAX_STRING) == 0) edge = EDGE_BOTH;
        free (edge_val);
    }
    jerry_release_value (string_value);

    if ((edge == EDGE_RISING && val == 1) || (edge == EDGE_FALLING && val == 0) || (edge == EDGE_BOTH)){
        jerry_value_t args[2];
        args[0] = jerry_create_number (val);
        args[1] = jerry_create_undefined ();
        jerry_value_t this_val = jerry_create_undefined ();
        jerry_release_value (jerry_call_function (fct, this_val, args, 2));
        jerry_release_value (args[0]);
        jerry_release_value (args[1]);
        jerry_release_value (this_val);
    }
}

static jerry_value_t button_watch_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    if (arguments_count > 0 && jerry_value_is_function (arguments[0])){
        jerry_value_t js_callback_function = jerry_acquire_value(arguments[0]);

        jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "_edge");
        jerry_value_t edge = jerry_get_property(function_this, prop_name);
        jerry_release_value (prop_name);

        int pin = get_pin (function_this);
        int ret_val = button_enable_interrupt (pin);
        if (ret_val == TOCK_SUCCESS){
            button_cb_t *ud = malloc (sizeof(button_cb_t));
            ud->cb_function = js_callback_function;
            ud->edge = edge;
            ret_val = button_subscribe(button_callback, ud);
            if (ret_val != TOCK_SUCCESS){
                jerry_value_t args[2];
                args[0] = jerry_create_undefined();
                char *error = (char*)malloc(100 * sizeof(char));
                if (error != NULL){
                    sprintf(error, "Could not subscribe to button callback, error: %d", ret_val);
                    args[1] = jerry_create_error (JERRY_ERROR_COMMON, (const jerry_char_t) error);

                    jerry_value_t this_val = jerry_create_undefined ();
                    jerry_release_value (jerry_call_function (js_callback_function, this_val, args, 2));
                    free (error);
                    jerry_release_value (args[0]);
                    jerry_release_value (args[1]);
                    jerry_release_value (this_val);
                }
                jerry_release_value (js_callback_function);
                jerry_release_value (edge);
                free (ud);
            }
        }
        else{
            jerry_value_t args[2];
            args[0] = jerry_create_undefined();
            char *error = (char*)malloc(100 * sizeof(char));
            if (error != NULL){
                sprintf(error, "Could not enable button interrupt, error: %d", ret_val);
                args[1] = jerry_create_error (JERRY_ERROR_COMMON, (const jerry_char_t) error);
                jerry_value_t this_val = jerry_create_undefined ();
                jerry_release_value (jerry_call_function (js_callback_function, this_val, args, 2));
                free (error);
                jerry_release_value (args[0]);
                jerry_release_value (args[1]);
                jerry_release_value (this_val);
            }
            jerry_release_value (js_callback_function);
            jerry_release_value (edge);
        }
    }
    return jerry_create_undefined();
}

static jerry_value_t button_read_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    return jerry_create_number (button_read (pin));
}

static jerry_value_t button_unwatch_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    int pin = get_pin (function_this);
    return jerry_create_number (button_disable_interrupt (pin));
}

static jerry_value_t button_edge_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "_edge");
    jerry_value_t prop_value = jerry_get_property(function_this, prop_name);
    jerry_release_value (prop_name);

    return prop_value;
}

static jerry_value_t button_set_edge_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    if (arguments_count > 0){
        jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "_edge");
        jerry_set_property (function_this, prop_name, arguments[0]);
        jerry_release_value (prop_name);
    }
    return jerry_create_undefined ();
}

static jerry_value_t new_button_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){
    jerry_value_t new_target = jerry_get_new_target ();
    if (jerry_value_get_type (new_target) == JERRY_TYPE_FUNCTION){
        if (arguments_count > 0){
            jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "_pin");
            jerry_release_value (jerry_set_property (function_this, prop_name, arguments[0]));
            jerry_release_value (prop_name);

            prop_name = jerry_create_string ((const jerry_char_t *) "_edge");
            if (arguments_count > 1){
                jerry_release_value (jerry_set_property (function_this, prop_name, arguments[1]));  
            }
            else{
                jerry_value_t edge = jerry_create_string ((const jerry_char_t *) "none");
                jerry_release_value (jerry_set_property (function_this, prop_name, edge));
                jerry_release_value (edge);
            }
            jerry_release_value (prop_name);

            prop_name = jerry_create_string ((const jerry_char_t *) "watch");
            jerry_value_t func_obj = jerry_create_external_function (button_watch_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "readSync");
            func_obj = jerry_create_external_function (button_read_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "unwatch");
            func_obj = jerry_create_external_function (button_unwatch_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "edge");
            func_obj = jerry_create_external_function (button_edge_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);

            prop_name = jerry_create_string ((const jerry_char_t *) "setEdge");
            func_obj = jerry_create_external_function (button_set_edge_handler);
            jerry_release_value (jerry_set_property (function_this, prop_name, func_obj));
            jerry_release_value (prop_name);
            jerry_release_value (func_obj);
        }
    }
    jerry_release_value (new_target);

    return jerry_create_undefined ();
}

jerry_value_t setup_button () {
    jerry_value_t button_object = jerry_create_external_function (new_button_handler);

    if (!jerry_value_is_error(button_object)){
        jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "count");
        jerry_value_t func_obj = jerry_create_external_function (button_count_handler);
        jerry_release_value (jerry_set_property (button_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);
	}
    return button_object;
}