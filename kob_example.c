
#include <stddef.h>
#define KOB_IMPL
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

  kob_print_heap_in_chunks();
  kob_print_heap();

  FILE *test_file = fopen("Makefile", "r");
  if (!test_file) {

    kob_printf("Failed to open file");
    return 1;
  }

  fseek(test_file, 0, SEEK_END);
  long fsize = ftell(test_file);
  fseek(test_file, 0, SEEK_SET);

  kob_String buffer = String_with_size(fsize);
  fread((void *)String_cstr(&buffer), fsize, 1, test_file);
  fclose(test_file);
  String_push(&buffer, '\0');

  kob_printf("%.*s\n", str_print_layout(String_as_str(&buffer)));

  const size_t max_lines = 100;
  kob_str lines[max_lines];
  const size_t num_lines = String_split_on(&buffer, lines, '\n', max_lines);
  for (size_t i = 0; i < num_lines; i++) {
    kob_printf("Line: %ld, %.*s\n", i, str_print_layout(lines[i]));
  }

  kob_print_heap_in_chunks();
  kob_print_heap();

  String_destroy(&buffer);
  return 0;
}
