#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int pid = getpid();
  printf("getpid returned %d\n", pid);
  exit(0);
}