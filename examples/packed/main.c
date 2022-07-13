#include <led.h>
#include <timer.h>
#include <gpio.h>

// use packed system calls
#define PACKED

// gpio driver 4, led driver 2
// when using the gpio driver, make sure no to change the GPIO_PIN
#define DRIVER 2

#define SYSCALLS_NUMBER 100

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

int main(void) {

  #ifdef PACKED
  uint32_t data[SYSCALLS_NUMBER*5];
  // printf("Packed syscalls, driver: %d, syscalls number: %d\n", DRIVER, SYSCALLS_NUMBER);
  #endif

  bool light = true;

  gpio_enable_output(GPIO_PIN);

  while (true) {

    command(4, 2, GPIO_PIN, 0);

    #ifdef PACKED
    for (int i=0; i < SYSCALLS_NUMBER; i++) {
      data[0 + 5*i] = 2;
      data[1 + 5*i] = DRIVER;
      data[2 + 5*i] = (light?2:3);
      data[3 + 5*i] = 2;
      data[4 + 5*i] = 0;
    }
    syscall_return_t res = packed(SYSCALLS_NUMBER, data, true);
    #endif

    // RISC-V Gpio 25 152 us
    // RISC-V Gpio 50 296 us
    // RISC-V Gpio 100 580 us
    // RISC-V Led 25 136us
    // RISC-V Led 50 256us
    // RISC-V Led 100 408us - 520us
    // ARM Gpio 25 690us
    // ARM Gpio 50 1.340ms
    // ARM Gpio 100 2.64ms
    // ARM Led 25 720us
    // ARM Led 50 1.440ms
    // ARM Led 100 2.800ms

    #ifndef PACKED
    for (int i=0; i < SYSCALLS_NUMBER; i++) {
      command (DRIVER, (light?2:3), 2, 0);
    }
    #endif

    // RISC-V Gpio 25 188us
    // RISC-V Gpio 50 360us
    // RISC-V Gpio 100 736us
    // RISC-V Led 25 170us
    // RISC-V Led 50 336us
    // RISC-V Led 100 660us
    // ARM Gpio 25 740us
    // ARM Gpio 50 1.46ms
    // ARM Gpio 100 2.96ms
    // ARM Led 25 760us
    // ARM Led 50 1.520ms
    // ARM Led 100 3.040ms

    command(4, 3, GPIO_PIN, 0);

    // int executed_syscalls = 25;

    // if (res.type == TOCK_SYSCALL_FAILURE_U32) {
    //   printf ("Executed %ld syscalls\n", 25 - res.data[0]); 
    //   executed_syscalls = 25 - res.data[0];
    // }

    light = !light;

    // for (int i = 0; i < executed_syscalls; i++) {
    //   printf ("syscall %d res %ld\n", i, data[1 + 5*i]);
    // }

    // This delay uses an underlying timer in the kernel.
    delay_ms(5);
  }
}
