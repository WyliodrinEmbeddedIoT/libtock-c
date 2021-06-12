#include <stdio.h>
#include <console.h>
#include <temperature.h>
#include <ninedof.h>

static int read_line (char *buffer, int len)
{
  int n=0;
  char v = 0;
  do 
  {
    v = getch ();
    // printf ("%c", v);
    // fflush (stdout);
    buffer[n] = v;
    n = n + 1;
  } while (v != '\n' && n < len);
  buffer[n-1] = 0;
  return n;
}

int main(void) {
  char buffer[100];
  int temp;
  int ninedof_accel_x = 0, ninedof_accel_y = 0, ninedof_accel_z = 0;
  while (1) {
    read_line (buffer, 99);
    // printf ("%s\n", buffer);
    if (strncmp (buffer, "temp", 4) == 0) {
      temperature_read_sync(&temp);
      printf ("temp: %d\n", temp);
    }
    else
    if (strncmp (buffer, "accel", 5) == 0) {
      ninedof_read_acceleration_sync(&ninedof_accel_x, &ninedof_accel_y, &ninedof_accel_z);
      printf ("x: %d y: %d z: %d\n", ninedof_accel_x, ninedof_accel_y, ninedof_accel_z);
    }
  }
}
