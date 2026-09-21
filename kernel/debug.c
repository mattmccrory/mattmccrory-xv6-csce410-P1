#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "debug.h"

// Start with logging disabled.
uint64 dbg_mask = 0;

// Maximum severity level to display when logging is enabled.
int dbg_level = DBG_INFO;

// Return the printable name for a debug category.
const char *
debug_cat_name(uint64 cat)
{
  switch (cat) {
  case DBG_PROC:
    return "PROC";
  case DBG_SCHED:
    return "SCHED";
  case DBG_SYSCALL:
    return "SYSCALL";
  case DBG_TRAP:
    return "TRAP";
  case DBG_VM:
    return "VM";
  case DBG_FS:
    return "FS";
  case DBG_DEV:
    return "DEV";
  default:
    return "UNKNOWN";
  }
}

// Return the current process ID, or -1 if there is no current process.
int
debug_pid(void)
{
  struct proc *p = myproc();

  if (p == 0)
    return -1;

  return p->pid;
}