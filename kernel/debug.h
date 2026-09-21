#ifndef KERNEL_DEBUG_H
#define KERNEL_DEBUG_H

#include "types.h"

// Debugging categories
#define DBG_PROC     (1UL << 0)
#define DBG_SCHED    (1UL << 1)
#define DBG_SYSCALL  (1UL << 2)
#define DBG_TRAP     (1UL << 3)
#define DBG_VM       (1UL << 4)
#define DBG_FS       (1UL << 5)
#define DBG_DEV      (1UL << 6)
#define DBG_ALL      (~0UL)

// Debugging severity levels
#define DBG_ERR      1
#define DBG_WARN     2
#define DBG_INFO     3
#define DBG_TRACE    4

// Global logging configuration
extern uint64 dbg_mask;
extern int dbg_level;

// Helpers implemented in debug.c
const char *debug_cat_name(uint64 cat);
int debug_pid(void);

#ifdef DEBUG_LOGGING
#define dprintf(cat, level, fmt, ...)                              \
  do {                                                             \
    if (((cat) & dbg_mask) && ((level) <= dbg_level))              \
      printk("[DBG:%s] pid=%d %s: " fmt "\n",                       \
             debug_cat_name(cat), debug_pid(), __func__,           \
             ##__VA_ARGS__);                                       \
  } while (0)
#else
#define dprintf(cat, level, fmt, ...) do { } while (0)
#endif

#endif // KERNEL_DEBUG_H