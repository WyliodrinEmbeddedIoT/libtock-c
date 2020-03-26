#include "jerryscript.h"
#include "jerry_library.h"
#include "jerry_console.h"

double jerry_port_get_current_time (void){
    return 0;
}

double jerry_port_get_local_time_zone_adjustment (double unix_ms, bool is_utc){
    return 0;
}

void JERRY_ATTR_NORETURN jerry_port_fatal (jerry_fatal_code_t code){
    return 0;
}

size_t jerry_port_normalize_path (const char *in_path_p,
                                  char *out_buf_p,
                                  size_t out_buf_size,
                                  char *base_file_p){
    return 0;
}

jerry_value_t jerry_port_get_native_module (jerry_value_t name){
    return "undefined";
}

uint8_t *jerry_port_read_source (const char *file_name_p, size_t *out_size_p){
    return NULL;
}

void jerry_port_release_source (uint8_t *buffer_p){
    return 0;
}

static jerry_value_t require_handler (const jerry_value_t function_object,
               const jerry_value_t function_this,
               const jerry_value_t arguments[],
               const jerry_length_t arguments_count) /**< number of function arguments */
{
    /* There should be at least one argument */
  if (arguments_count > 0)
  {
    char *library_name = NULL;
    /* Convert the first argument to a string (JS "toString" operation) */
    jerry_value_t string_value = jerry_value_to_string (arguments[0]);
    jerry_size_t string_size = jerry_get_string_size (arguments[0]);

    library_name = (char*) malloc (string_size + 1);

    // if (library_name!=NULL)
	// {
	// 	int n = jerry_substring_to_char_buffer(string_value, 0, string_size, (jerry_char_t*)library_name, string_size);
	// 	library_name[n] = '\0';

    //     jerry_value_t ret_val;

    //     if (strncmp (library_name, "lib", JERRY_MAX_STRING) == 0)
    //     {
    //         ret_val = setup_lib ();
    //     }
    //     //else if ....
    //     free(library_name);
    //     return ret_val;
	// }

    jerry_release_value(string_value);
  }
  return jerry_create_undefined ();
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


        jerry_value_t property_name_console = jerry_create_string ((const jerry_char_t *) "console");
        jerry_value_t console = setup_console();
        jerry_release_value (jerry_set_property (global_object, property_name_console, console));
        jerry_release_value (property_name_console);
        jerry_release_value (console);
    }
	jerry_release_value (global_object);
}

void jerry_execute (const char *source)
{
    jerry_value_t parsed_code = jerry_parse (NULL, 0, source, strlen (source) - 1, JERRY_PARSE_NO_OPTS);

    if (!jerry_value_is_error (parsed_code))
    {
        jerry_release_value (jerry_run (parsed_code));
    }
    jerry_release_value (parsed_code);
}

void jerry_release ()
{
    jerry_cleanup();
}
