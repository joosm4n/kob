
#define KOB_IMPL
#define KOB_NO_STD
#define KOB_HEAP
#include "kob.h"

int main() {

  const char base[] = "Hello World";
  kob_String msg = String_from(base);
  kob_str str = String_slice(&msg, 0, KOB_COUNTOF(base));
  kob_str hello = str_slice(str, 0, 5);
  kob_str world = str_slice(str, 6, -1);
  kob_printf("%.*s\n", str_print_layout(str));

  kob_printf("%.*s\n", str_print_layout(hello));
  kob_printf("%.*s\n", str_print_layout(world));
  return 0;
}
