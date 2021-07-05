#include <stdio.h>
#include <tock.h>
#include <timer.h>

#define DRIVER_NUM_LATENCY 0x10001

char prog[] = {
        0xb7, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x73, 0x21, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xb7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  
int main(void) 
{
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_LATENCY, 1, (const void*) prog, 32);
  printf("A intors allow_readonly: %d", tock_allow_ro_return_to_returncode(aval));
	printf("Inceput de main");
	syscall_return_t res = command(DRIVER_NUM_LATENCY, 1, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
  	printf("Success!!");
  } else {
  	printf("Fails.. :( !!");
    return tock_status_to_returncode (res.data[0]);
  }
  return 0;
}