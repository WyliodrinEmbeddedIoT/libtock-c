#include <stdio.h>
#include <tock.h>
#include <timer.h>

#define DRIVER_NUM_LATENCY 0x08
int latency_start (void);
int latency_stop (void);

int command_subscribe = 0;
int command_command = 0;

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} LatencyTestReturn;

static int latency_command (int command_num, int data1, int data2);
int comm_sub (void);

static void com_sub_callback(int error,
                                 int data1,
                                 int data2,
                                 void* ud) {
  LatencyTestReturn *ret = (LatencyTestReturn*) ud;
  ret->error = error;
  ret->data1 = data1;
  ret->data2 = data2;
  ret->done  = true;
  syscall_return_t res = command(DRIVER_NUM_LATENCY, 3, 0, 0);
  if (res.type == TOCK_SYSCALL_SUCCESS_U32)
    command_subscribe = res.data[0]; 
  else 
    command_subscribe = -1;
}

static int latency_subscribe (subscribe_upcall cb, void *userdata) {
  subscribe_return_t sv = subscribe(DRIVER_NUM_LATENCY, 0, cb, userdata);
  if (sv.success == 0) {
    return tock_error_to_rcode(sv.error);
  }
  return TOCK_SUCCESS;
}

static int latency_command (int command_num, int data1, int data2) {
  syscall_return_t res = command(DRIVER_NUM_LATENCY, command_num, data1, data2);
  if (res.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode (res.data[0]);
  }
}

int main(void) 
{
  // printf("Test print\r\n");
  // delay_ms(5000);

  // command to command
  LatencyTestReturn ltr;
  ltr.error = latency_command(2, 0, 0);
  if (ltr.error == TOCK_SUCCESS) {
    syscall_return_t res = command(DRIVER_NUM_LATENCY, 3, 0, 0);
    if (res.type == TOCK_SYSCALL_SUCCESS_U32)
      command_command = res.data[0]; 
    else 
      command_command = -1;
  } else 
    command_command = -1;

  // command to subscribe_callback
  ltr.done  = false;
  ltr.error = latency_subscribe (com_sub_callback, &ltr);
  if (ltr.error == TOCK_SUCCESS) {
    ltr.error = latency_command (1, 0, 0);
    if (ltr.error == TOCK_SUCCESS) yield_for (&ltr.done);
  }

  printf("command_command: %d us; command_subscribe: %d us\r\n", command_command, command_subscribe);
  return 0;
}