#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int r;

  // Establish a known configuration:
  // mask = DBG_PROC (1), level = DBG_INFO (3).
  debugctl(1, 1);
  debugctl(3, 3);

  printf("Initial mask: %d (expected 1)\n", (int)debugctl(0, 0));
  printf("Initial level: %d (expected 3)\n", (int)debugctl(2, 0));

  // Test invalid operation.
  r = (int)debugctl(99, 0);
  printf("Invalid op return: %d (expected -1)\n", r);

  printf("Mask after invalid op: %d (expected 1)\n",
         (int)debugctl(0, 0));
  printf("Level after invalid op: %d (expected 3)\n",
         (int)debugctl(2, 0));

  // Test invalid levels: below minimum and above maximum.
  r = (int)debugctl(3, 0);
  printf("Set level 0 return: %d (expected -1)\n", r);

  printf("Level after setting 0: %d (expected 3)\n",
         (int)debugctl(2, 0));

  r = (int)debugctl(3, 5);
  printf("Set level 5 return: %d (expected -1)\n", r);

  printf("Level after setting 5: %d (expected 3)\n",
         (int)debugctl(2, 0));

  // Confirm mask was not changed by invalid-level requests.
  printf("Final mask: %d (expected 1)\n", (int)debugctl(0, 0));

  exit(0);
}