
#define KOB_IMPL
#include "kob.h"

int main() {

  const char base[] = "Hello World";
  kob_String msg = String_from(base);
  kob_str str = String_slice(&msg, 0, KOB_COUNTOF(base));
  kob_str hello = str_slice(str, 0, 5);
  kob_str world = str_slice(str, 6, -1);
  printf("%.*s\n", str_print_layout(str));

  printf("%.*s\n", str_print_layout(hello));
  printf("%.*s\n", str_print_layout(world));
  return 0;
}
