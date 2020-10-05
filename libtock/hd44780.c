#include "gpio.h"
#include "hd44780.h"
#include "text_screen.h"
#include "tock.h"

#define ALLOW_BAD_VALUE 200

// #define WIDTH 16
// #define HEIGHT 2

// #define LCD_CLEARDISPLAY 0x01
// #define LCD_DISPLAYON 0x04
// #define LCD_BLINKON 0x01
// #define LCD_CURSORON 0x02
// #define LCD_CURSORSHIFT 0x10
// #define LCD_DISPLAYMOVE 0x08
// #define LCD_ENTRYLEFT 0x02
// #define LCD_ENTRYSHIFTINCREMENT 0x01
// #define LCD_MOVELEFT 0x00
// #define LCD_MOVERIGHT 0x04

/* hd44780_init initializes the communication between the userspace and the
 * screen driver
 * 
 * As argument, there is the length of the buffer through which is sent the
 * data to be written on the LCD display is sent.
 *
 * Example:
 *  hd44780_init(8);
 */
int hd44780_init(uint8_t len)
{
  return text_screen_init(len);
}

/* hd44780_clear clears the display and sets the cursor position to zero
 *
 * Example:
 *  hd44780_clear(void);
 */
int hd44780_clear(void)
{
  return text_clear_screen();
}

/* hd44780_home, as hd44780_clear, clears the display and sets the cursor posiziton to zero
 *
 * Example:
 *  hd44780_home(void);
 */
int hd44780_home(void)
{
  return text_home();
}

/* hd44780_no_display turns the display off very quickly
 *
 * Example:
 *  hd44780_no_display(void);
 */
int hd44780_no_display(void)
{
  return text_display_off();
}

/* hd44780_display turns the display on very quickly
 *
 * Example:
 *  hd44780_display(void);
 */
int hd44780_display(void)
{
  return text_display_on();
}

/* hd44780_no_blink turns off the blinking cursor display
 *
 * Example:
 *  hd44780_no_blink(void);
 */
int hd44780_no_blink(void)
{
  return text_blink_off();
}

/* hd44780_blink turns on the blinking cursor display
 *
 * Example:
 *  hd44780_blink(void);
 */
int hd44780_blink(void)
{
  return text_blink_on();
}

/* hd44780_no_cursor turns off the underline cursor
 *
 * Example:
 *  hd44780_no_cursor(void);
 */
int hd44780_no_cursor(void)
{
  return text_hide_cursor();
}

/* hd44780_cursor turns on the underline cursor
 *
 * Example:
 *  hd44780_cursor(void);
 */
int hd44780_cursor(void)
{
  return text_show_cursor();
}

/* hd44780_scroll_display_left scrolls the display to the left without
 * changing the RAM
 *
 * Example:
 *  hd44780_scroll_display_left(void);
 */
int hd44780_scroll_display_left(void)
{
  return -1;
  // return screen_driver_command(3, 0, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/* hd44780_scroll_display_right scrolls the display to the right without
 * changing the RAM
 *
 * Example:
 *  hd44780_scroll_display_right(void);
 */
int hd44780_scroll_display_right(void)
{
  return -1;
  // return screen_driver_command(3, 0, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/* hd44780_left_to_right should flow the text from left to right on the display
 *
 * Example:
 *  hd44780_left_to_right(void);
 */
int hd44780_left_to_right(void)
{
  return -1;
  // return screen_driver_command(1, 0, LCD_ENTRYLEFT);
}

/* hd44780_right_to_left should flow the text from right to left on the display
 *
 * Example:
 *  hd44780_right_to_left(void);
 */
int hd44780_right_to_left(void)
{
  return -1;
  // return screen_driver_command(1, 1, LCD_ENTRYLEFT);
}

/* hd44780_autoscroll will 'right justify' the text from the cursor
 *
 * Example:
 *	hd44780_autoscroll(void);
 */
int hd44780_autoscroll(void)
{
  return -1;
  // return screen_driver_command(1, 0, LCD_ENTRYSHIFTINCREMENT);
}

/* hd44780_no_autoscroll will 'left justify' the text from the cursor
 *
 * Example:
 *	hd44780_no_autoscroll(void);
 */
int hd44780_no_autoscroll(void)
{
  return -1;
  // return screen_driver_command(1, 1, LCD_ENTRYSHIFTINCREMENT);
}

/* hd44780_print_full_string will do some checks for the number of bytes to
 * be sent and will try to send to the kernel all the bytes in the buffer.
 * When the syscall returns a certain value (ALLOW_BAD_VALUE) it means that the
 * command buffer in the kernel is full and a delay is needed in order for the
 * buffer to be emptied (the commands already saved to be actually executed).
 * As argument, there is the string to be printed.
 *
 * Example:
 *	hd44780_print_full_string("Here I am!");
 */
uint8_t hd44780_print_full_string(char* string)
{
  char buff[128];
  uint8_t* buffer = text_buffer();
  uint8_t len = strlen(string);
  uint8_t n   = 0;
  while (n < len) {
    uint8_t to_write = ((uint8_t) sizeof(buff) > len - n) ? len - n : (uint8_t) sizeof(buff);
    memcpy(buffer, string, to_write);
    int ret = text_write(to_write);
    // int ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
    if (ret == ALLOW_BAD_VALUE) {
      while (ret == ALLOW_BAD_VALUE) {
        delay_ms(1000);
        ret = text_write(to_write);
        // ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
      }
    }
    n      += ret;
    string += ret;
  }
  return n;
}

/* hd44780_print_string will do some checks for the number of bytes to be sent
 * and will try to send to the kernel all the bytes in the buffer.
 * When the syscall returns a certain value (ALLOW_BAD_VALUE) it stops.
 * As argument, there is the string to be printed.
 *
 * Example:
 *	hd44780_print_string("Here I am!");
 */
uint8_t hd44780_print_string(const char* string)
{
  char buff[128];
  uint8_t* buffer = text_buffer();
  uint8_t len = strlen(string);
  uint8_t n   = 0;
  while (n < len) {
    uint8_t to_write = ((uint8_t) sizeof(buff) > len - n) ? len - n : (uint8_t) sizeof(buff);
    memcpy(buffer, string, to_write);
    int ret = text_write(to_write);
    // int ret = allow(DRIVER_HD44780_NUM, 0, (void* )buff, to_write);
    if (!ret || ret == ALLOW_BAD_VALUE) return n;
    n      += ret;
    string += ret;
  }
  return n;
}

/* hd44780_set_cursor will set the cursor position to the row and col given as
 * arguments
 * As arguments, there are:
 * - the column
 * - the line
 *
 * Example:
 *	hd44780_set_cursor(5, 1);
 */

int hd44780_set_cursor(uint8_t col, uint8_t row)
{
  return text_set_cursor(col, row);
}
