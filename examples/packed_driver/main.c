#include <stdlib.h>

#include <led.h>
#include <timer.h>
#include <gpio.h>

// use packed system calls
#define PACKED

#define BUFFERS_RO 10
#define BUFFERS_RW 10

#define SYSCALLS_NUMBER (BUFFERS_RO+BUFFERS_RW + 1 + 1 + 1 + 1 + BUFFERS_RO+BUFFERS_RW)

#if defined(__thumb__)
#define GPIO_PIN 16
static syscall_return_t packed(uint32_t count, uint32_t *data,
                         bool stop_error) {
  register uint32_t r0 __asm__ ("r0") = count;
  register uint32_t *r1 __asm__ ("r1") = data;
  register uint32_t r2 __asm__ ("r2") = stop_error;
  register uint32_t r3 __asm__ ("r3") = 0;
  register uint32_t rtype __asm__ ("r0");
  register uint32_t rv1 __asm__ ("r1");
  register uint32_t rv2 __asm__ ("r2");
  register uint32_t rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 0xfe"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}
#elif defined(__riscv)
#define GPIO_PIN 0
static syscall_return_t packed(uint32_t count, uint32_t *data,
                         bool stop_error) {
  register uint32_t a0  __asm__ ("a0") = count;
  register uint32_t *a1  __asm__ ("a1") = data;
  register uint32_t a2  __asm__ ("a2") = stop_error;
  register uint32_t a3  __asm__ ("a3") = 0;
  register uint32_t a4  __asm__ ("a4") = 0xfe;
  register int rtype __asm__ ("a0");
  register int rv1 __asm__ ("a1");
  register int rv2 __asm__ ("a2");
  register int rv3 __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}
#endif

static packed_allow_readonly(uint32_t *data, int index, uint32_t driver_number, uint32_t buffer_number, unsigned char *buffer, uint32_t len) {
  data[0 + 5*index] = 4;
  data[1 + 5*index] = driver_number;
  data[2 + 5*index] = buffer_number;
  data[3 + 5*index] = (uint32_t)buffer;
  data[4 + 5*index] = len;
}

static packed_allow_readwrite(uint32_t *data, int index, uint32_t driver_number, uint32_t buffer_number, unsigned char *buffer, uint32_t len) {
  data[0 + 5*index] = 3;
  data[1 + 5*index] = driver_number;
  data[2 + 5*index] = buffer_number;
  data[3 + 5*index] = (uint32_t)buffer;
  data[4 + 5*index] = len;
}

static packed_subscribe(uint32_t *data, int index, uint32_t driver_number, uint32_t subscribe_number, subscribe_upcall cb, void* userdata) {
  data[0 + 5*index] = 1;
  data[1 + 5*index] = driver_number;
  data[2 + 5*index] = subscribe_number;
  data[3 + 5*index] = (uint32_t)cb;
  data[4 + 5*index] = (uint32_t)userdata;
}

static packed_command(uint32_t *data, int index, uint32_t driver_number, uint32_t command_number, uint32_t arg1, uint32_t arg2) {
  data[0 + 5*index] = 2;
  data[1 + 5*index] = driver_number;
  data[2 + 5*index] = command_number;
  data[3 + 5*index] = arg1;
  data[4 + 5*index] = arg2;
}

static packed_yield(uint32_t *data, int index, uint32_t which) {
  data[0 + 5*index] = 0;
  data[1 + 5*index] = which;
  // data[2 + 5*index] = 0;
  // data[3 + 5*index] = 0;
  // data[4 + 5*index] = 0;
}

static void callback (int r0, int r1, int r2, void *r3) {
  printf("callback r0: %d, r1: %d, r2: %d, r3: %p\n", r0, r1, r2, r3);
}

int main(void) {

  unsigned char *buffers_ro[BUFFERS_RO];
  unsigned char *buffers_rw[BUFFERS_RW];

  for (int i=0; i<BUFFERS_RO; i++) {
    buffers_ro[i] = (unsigned char *)malloc (sizeof(char)*(i+1)*10);
  }

  for (int i=0; i<BUFFERS_RW; i++) {
    buffers_rw[i] = (unsigned char *)malloc (sizeof(char)*(i+1)*10);
  }


  #ifdef PACKED
  uint32_t data[SYSCALLS_NUMBER*5];
  // printf("Packed syscalls, syscalls number: %d\n", SYSCALLS_NUMBER);
  #endif

  gpio_enable_output(GPIO_PIN);

  // while (true) {

    command(4, 2, GPIO_PIN, 0);

    #ifdef PACKED
    // allow_ro
    for (int i=0; i < BUFFERS_RO; i++) {
      // data[0 + 5*i] = 4;
      // data[1 + 5*i] = 0xa0000;
      // data[2 + 5*i] = i;
      // data[3 + 5*i] = buffers_ro[i];
      // data[4 + 5*i] = sizeof(char)*(i+1)*10;
      packed_allow_readonly(data, i, 0xa0000, i, buffers_ro[i], sizeof(char)*(i+1)*10);
    }

    // allow_rw
    for (int i=0; i < BUFFERS_RW; i++) {
      // data[0 + 5*i] = 3;
      // data[1 + 5*i] = 0xa0000;
      // data[2 + 5*i] = i - BUFFERS_RO;
      // data[3 + 5*i] = buffers_rw[i - BUFFERS_RO];
      // data[4 + 5*i] = sizeof(char)*(i-BUFFERS_RO+1)*10;
      packed_allow_readwrite(data, BUFFERS_RO + i, 0xa0000, i, buffers_ro[i], sizeof(char)*(i+1)*10);
    }

    // subscribe
    // data[0 + 5*(BUFFERS_RO+BUFFERS_RW)] = 1;
    // data[1 + 5*(BUFFERS_RO+BUFFERS_RW)] = 0xa0000;
    // data[2 + 5*(BUFFERS_RO+BUFFERS_RW)] = 0;
    // data[3 + 5*(BUFFERS_RO+BUFFERS_RW)] = callback;
    // data[4 + 5*(BUFFERS_RO+BUFFERS_RW)] = 0;
    packed_subscribe(data, BUFFERS_RO+BUFFERS_RW, 0xa0000, 0, callback, 0xfa);

    // command
    // data[0 + 5*(BUFFERS_RO+BUFFERS_RW+1)] = 2;
    // data[1 + 5*(BUFFERS_RO+BUFFERS_RW+1)] = 0xa0000;
    // data[2 + 5*(BUFFERS_RO+BUFFERS_RW+1)] = 1;
    // data[3 + 5*(BUFFERS_RO+BUFFERS_RW+1)] = 0;
    // data[4 + 5*(BUFFERS_RO+BUFFERS_RW+1)] = 0;
    packed_command(data, BUFFERS_RO+BUFFERS_RW+1, 0xa0000, 1, 0xfd, 0xfe);

    // yield
    // data[0 + 5*(BUFFERS_RO+BUFFERS_RW+2)] = 0;
    // data[1 + 5*(BUFFERS_RO+BUFFERS_RW+2)] = 1;
    // data[2 + 5*(BUFFERS_RO+BUFFERS_RW+2)] = 0;
    // data[3 + 5*(BUFFERS_RO+BUFFERS_RW+2)] = 0;
    // data[4 + 5*(BUFFERS_RO+BUFFERS_RW+2)] = 0;
    packed_yield(data, BUFFERS_RO+BUFFERS_RW+2, 1);
    

    // unsubscribe
    // data[0 + 5*(BUFFERS_RO+BUFFERS_RW+3)] = 1;
    // data[1 + 5*(BUFFERS_RO+BUFFERS_RW+3)] = 0xa0000;
    // data[2 + 5*(BUFFERS_RO+BUFFERS_RW+3)] = 0;
    // data[3 + 5*(BUFFERS_RO+BUFFERS_RW+3)] = NULL;
    // data[4 + 5*(BUFFERS_RO+BUFFERS_RW+3)] = 0;
    packed_subscribe(data, BUFFERS_RO+BUFFERS_RW+3, 0xa0000, 0, NULL, 0);

    // unallow_ro
    for (int i=0; i < BUFFERS_RO; i++) {
      // data[0 + 5*i] = 4;
      // data[1 + 5*i] = 0xa0000;
      // data[2 + 5*i] = i - (BUFFERS_RO+BUFFERS_RW+4);
      // data[3 + 5*i] = NULL;
      // data[4 + 5*i] = 0;
      packed_allow_readonly(data, BUFFERS_RO+BUFFERS_RW+4+i, 0xa0000, i, NULL, 0);
    }

    // unallow_rw
    for (int i=0; i < BUFFERS_RW; i++) {
      // data[0 + 5*i] = 3;
      // data[1 + 5*i] = 0xa0000;
      // data[2 + 5*i] = i - (BUFFERS_RO+BUFFERS_RW+4+BUFFERS_RO);
      // data[3 + 5*i] = NULL;
      // data[4 + 5*i] = 0;
      packed_allow_readwrite(data, BUFFERS_RO+BUFFERS_RW+4+BUFFERS_RO + i, 0xa0000, i, NULL, 0);
    }

    syscall_return_t res = packed(SYSCALLS_NUMBER, data, true);
    #endif

    

    #ifndef PACKED
    // allow_ro
    for (int i=0; i < BUFFERS_RO; i++) {
      allow_readonly (0xa0000, i, buffers_ro[i], sizeof(char)*(i+1)*10);
    }

    // allow_rw
    for (int i=BUFFERS_RO; i < BUFFERS_RO+BUFFERS_RW; i++) {
      allow_readonly (0xa0000, i-BUFFERS_RO, buffers_rw[i-BUFFERS_RO], sizeof(char)*(i-BUFFERS_RO+1)*10);
    }

    subscribe(0xa0000, 0, callback, 0);
    command (0xa0000, 1, 0, 0);
    yield();

    subscribe(0xa0000, 0, NULL, 0);

    // allow_ro
    for (int i=0; i < BUFFERS_RO; i++) {
      allow_readonly (0xa0000, i, NULL, 0);
    }

    // allow_rw
    for (int i=0; i < BUFFERS_RW; i++) {
      allow_readonly (0xa0000, i, NULL, 0);
    }

    #endif


    command(4, 3, GPIO_PIN, 0);

    // int executed_syscalls = 25;

    // if (res.type == TOCK_SYSCALL_FAILURE_U32) {
    //   printf ("Executed %ld syscalls\n", 25 - res.data[0]); 
    //   executed_syscalls = 25 - res.data[0];
    // }

    // for (int i = 0; i < executed_syscalls; i++) {
    //   printf ("syscall %d res %ld\n", i, data[1 + 5*i]);
    // }

    // This delay uses an underlying timer in the kernel.
  //   delay_ms(5);
  // }
}
