#include "jerry_library.h"
#include <stdlib.h>

static jerry_value_t console_log_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count){ 
    if (arguments_count > 0){
        for (int i=0; i< arguments_count; i++){
            char *str = NULL;
            jerry_value_t string_value = jerry_value_to_string (arguments[0]);
            jerry_size_t string_size = jerry_get_string_size (string_value);

            str = (char*) malloc (string_size + 1);
            if (str!=NULL){
                int n = jerry_substring_to_char_buffer(string_value, 0, string_size, (jerry_char_t*)str, string_size);
                str[n] = '\0';

                printf ("%s", str);
            }         
            jerry_release_value(string_value);
        }
        printf ("\n");
    }
    return jerry_create_undefined ();
}

jerry_value_t setup_console (){
    jerry_value_t console_object = jerry_create_object ();

    if (!jerry_value_is_error(console_object)){
        jerry_value_t prop_name = jerry_create_string ((const jerry_char_t *) "log");
        jerry_value_t func_obj = jerry_create_external_function (console_log_handler);
        jerry_release_value (jerry_set_property (console_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        prop_name = jerry_create_string ((const jerry_char_t *) "error");
        func_obj = jerry_create_external_function (console_log_handler);
        jerry_release_value (jerry_set_property (console_object, prop_name, func_obj));
        jerry_release_value (prop_name);
        jerry_release_value (func_obj);

        return console_object;
    }
    return jerry_create_undefined();
}