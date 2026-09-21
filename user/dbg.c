#include "kernel/types.h"
#include "user.h"
#include "../kernel/debug.h"

struct category {
  char *name;
  uint64 bit;
};

static struct category categories[] = {
  {"proc",    DBG_PROC},
  {"sched",   DBG_SCHED},
  {"syscall", DBG_SYSCALL},
  {"trap",    DBG_TRAP},
  {"vm",      DBG_VM},
  {"fs",      DBG_FS},
  {"dev",     DBG_DEV},
};

#define NCATEGORIES (sizeof(categories) / sizeof(categories[0]))

static char *
level_name(int level)
{
  switch (level) {
  case DBG_ERR:   return "err";
  case DBG_WARN:  return "warn";
  case DBG_INFO:  return "info";
  case DBG_TRACE: return "trace";
  default:        return "unknown";
  }
}

static int
parse_level(char *name)
{
  if (strcmp(name, "err") == 0)   return DBG_ERR;
  if (strcmp(name, "warn") == 0)  return DBG_WARN;
  if (strcmp(name, "info") == 0)  return DBG_INFO;
  if (strcmp(name, "trace") == 0) return DBG_TRACE;
  return -1;
}

static int
parse_category(char *name)
{
  for (int i = 0; i < NCATEGORIES; i++) {
    if (strcmp(name, categories[i].name) == 0)
      return i;
  }

  return -1;
}

static void
usage(void)
{
  printf("Usage:\n");
  printf("  dbg\n");
  printf("  dbg on <category|all>\n");
  printf("  dbg off <category|all>\n");
  printf("  dbg only <category|all>\n");
  printf("  dbg level <err|warn|info|trace>\n");
}

static void
show_settings(void)
{
  uint64 mask = debugctl(DBG_GETMASK, 0);
  int level = debugctl(DBG_GETLEVEL, 0);

  printf("Debug categories enabled:");
  int any = 0;

  for (int i = 0; i < NCATEGORIES; i++) {
    if (mask & categories[i].bit) {
      printf(" %s", categories[i].name);
      any = 1;
    }
  }

  if (!any)
    printf(" none");

  printf("\nDebug level: %s\n", level_name(level));
}

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    show_settings();
    exit(0);
  }

  if (argc == 3 && strcmp(argv[1], "level") == 0) {
    int level = parse_level(argv[2]);

    if (level == -1) {
      usage();
      exit(1);
    }

    int old = debugctl(DBG_SETLEVEL, level);
    if (old == -1) {
      printf("dbg: failed to set level\n");
      exit(1);
    }

    show_settings();
    exit(0);
  }

  if (argc == 3 &&
      (strcmp(argv[1], "on") == 0 ||
       strcmp(argv[1], "off") == 0 ||
       strcmp(argv[1], "only") == 0)) {

    uint64 mask = debugctl(DBG_GETMASK, 0);
    uint64 bit;
    int idx = parse_category(argv[2]);

    if (strcmp(argv[2], "all") == 0) {
      bit = DBG_ALL;
    } else if (idx >= 0) {
      bit = categories[idx].bit;
    } else {
      usage();
      exit(1);
    }

    uint64 newmask;

    if (strcmp(argv[1], "on") == 0)
      newmask = mask | bit;
    else if (strcmp(argv[1], "off") == 0)
      newmask = mask & ~bit;
    else
      newmask = bit;

    if (strcmp(argv[1], "off") == 0 &&
        strcmp(argv[2], "all") == 0)
      newmask = 0;

    debugctl(DBG_SETMASK, newmask);

    show_settings();
    exit(0);
  }

  usage();
  exit(1);
}