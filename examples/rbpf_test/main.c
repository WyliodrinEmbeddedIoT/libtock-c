#include <stdio.h>
#include <tock.h>
#include <timer.h>

#define DRIVER_NUM_LATENCY 0x10001

int main(void) 
{
	printf("Inceput de main");
	syscall_return_t res = command(DRIVER_NUM_LATENCY, 0, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
  	printf("Success!!");
  } else {
  	printf("Fails.. :( !!");
    return tock_status_to_returncode (res.data[0]);
  }
  // printf("command_command: %d us; command_subscribe: %d us\r\n", command_command, command_subscribe);
  return 0;
}