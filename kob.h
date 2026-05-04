#ifndef KOB_HEADER_FILE
#define KOB_HEADER_FILE

#define KOB_IMPL

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#define KOB_DECLTYPE_CAST(T) (decltype(T))
#else
#define KOB_DECLTYPE_CAST(T)
#endif

#define KOB_PTR_CAST(type, obj) ((type)(&obj))
#define KOB_BIT_CAST(type, obj) (*(type *)(&obj))

#define KOB_PASTE(x, y) KOB_PASTE_IMPL(x, y)
#define KOB_PASTE_IMPL(x, y) x##y
#define KOB_STRINGIFY(x) #x

#define KOB_MEMSET memset
#define KOB_STRLEN strlen
#define KOB_FREE free
#define KOB_MALLOC malloc
#define KOB_CALLOC calloc
#define KOB_REALLOC realloc

#define KOB_COUNTOF(x) ((size_t)(sizeof(x) / sizeof(x[0])))
#define KOB_ZERO(x) (KOB_MEMSET(x, 0, KOB_COUNTOF(x)))

#define KOBDEF inline static

#define KOB_FOREACH(_item_, _array_)                                           \
  for (size_t i = 0u, _keep_ = 1u; _keep_ && i < KOB_COUNTOF(_array_);         \
       _keep_ = !_keep_, i++)                                                  \
    for (_item_ = _array_[i]; _keep_; _keep_ = !_keep_)

#define KOB_PRINTF printf
#define KOB_LOG_INFO(fmt, ...)                                                 \
  {                                                                            \
    KOB_PRINTF("[KOB-INFO]: ");                                                \
    KOB_PRINTF(fmt, ##__VA_ARGS__);                                            \
    KOB_PRINTF("\n");                                                          \
  }

#define KOB_LOG_ERROR(fmt, ...)                                                \
  {                                                                            \
    KOB_PRINTF("[KOB-ERROR] : ");                                              \
    KOB_PRINTF(fmt, ##__VA_ARGS__);                                            \
    KOB_PRINTF("\n");                                                          \
  }

#define KOB_LOG_ERROR2(fmt, ...)                                               \
  {                                                                            \
    KOB_PRINTF("[KOB-ERROR] : %s:%d \n          ERROR: ", __FILE__, __LINE__); \
    KOB_PRINTF(fmt, ##__VA_ARGS__);                                            \
    KOB_PRINTF("\n");                                                          \
  }

#define KOB_ASSERT(check, msg, ...)                                            \
  if (!(check)) {                                                              \
    KOB_PRINTF("[KOB-ASSERT] %s:%d: ", __FILE__, __LINE__);                    \
    KOB_PRINTF(msg, ##__VA_ARGS__);                                            \
    KOB_PRINTF("\n");                                                          \
    assert((check && msg));                                                    \
  }

#define TODO(msg)                                                              \
  {                                                                            \
    KOB_PRINTF("[KOB-TODO] %s:%d: ", __FILE__, __LINE__);                      \
    KOB_PRINTF(msg, ##__VA_ARGS__);                                            \
    KOB_PRINTF("\n");                                                          \
    assert((false && msg));                                                    \
  }

#define KOB_DA_INIT_CAP 32

#define kob_da_reserve(da, expected_capacity)                                  \
  do {                                                                         \
    if ((expected_capacity) > (da)->capacity) {                                \
      if ((da)->capacity == 0) {                                               \
        (da)->capacity = KOB_DA_INIT_CAP;                                      \
      }                                                                        \
      while ((expected_capacity) > (da)->capacity) {                           \
        (da)->capacity *= 2;                                                   \
      }                                                                        \
      (da)->items = KOB_DECLTYPE_CAST((da)->items)                             \
          KOB_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));     \
      KOB_ASSERT((da)->items != NULL, "Buy more RAM lol");                     \
    }                                                                          \
  } while (0)

#define kob_da_push(da, item)                                                  \
  do {                                                                         \
    kob_da_reserve((da), (da)->count + 1);                                     \
    (da)->items[(da)->count++] = (item);                                       \
  } while (0)

#define kob_da_push_many(da, new_items, new_items_count)                       \
  do {                                                                         \
    kob_da_reserve((da), (da)->count + (new_items_count));                     \
    memcpy((da)->items + (da)->count, (new_items),                             \
           (new_items_count) * sizeof(*(da)->items));                          \
    (da)->count += (new_items_count);                                          \
  } while (0)

#define kob_da_resize(da, new_size)                                            \
  do {                                                                         \
    kob_da_reserve((da), new_size);                                            \
    (da)->count = (new_size);                                                  \
  } while (0)

#define kob_da_pop(da) (da)->items[(KOB_ASSERT((da)->count > 0), --(da)->count)]
#define kob_da_first(da) (da)->items[(KOB_ASSERT((da)->count > 0), 0)]
#define kob_da_last(da) (da)->items[(da)->count - 1]

typedef struct kob_String kob_String;
typedef struct kob_str kob_str;

typedef struct kob_String {
  char *items;
  size_t capacity;
  size_t count;
} kob_String;

#define String_isvalid(s) ((s) != NULL ? (s)->items != NULL : 0)
#define String_len(s) ((s)->count)
#define String_first(s) (kob_da_first(s))
#define String_last(s) (kob_da_last(s))

typedef struct kob_str {
  const char *const *const ptr;
  const size_t start;
  const size_t len;
} kob_str;

#define str_isvalid(s) (s.ptr != NULL)
#define str_len(s) (s.len)
#define str_valid_index(s, idx) (s.ptr != NULL && idx < s.len)
#define str_at(s, idx) (str_valid_index(s, idx) ? (*s.ptr)[idx] : '\0')
#define str_print_layout(str) (int)str.len, (*str.ptr + str.start)

#ifdef __cplusplus

#define KOB_RULE_OF_FIVE(name)                                                 \
  KOB_PASTE(name, )() = default;                                               \
  KOB_PASTE(name, )(const KOB_PASTE(name, ) &) = delete;                       \
  KOB_PASTE(name, )(KOB_PASTE(name, ) &&) = delete;                            \
  KOB_PASTE(name, ) &operator=(const KOB_PASTE(name, ) &) = delete;            \
  KOB_PASTE(name, ) &operator=(KOB_PASTE(name, ) &&) = delete;                 \
  virtual ~KOB_PASTE(name, )() = default;

#else
#define KOB_RULE_OF_FIVE(name)

#endif // __cplusplus

#ifdef KOB_IMPL

//
// String
// vv
KOBDEF kob_String String_from(const char *c_str) {
  kob_String s = {};
  kob_String *s_ref = &s;
  size_t len = KOB_STRLEN(c_str);
  kob_da_push_many(s_ref, c_str, len);
  if (kob_da_last(s_ref) != '\0') {
    kob_da_push(s_ref, '\0');
  }
  return s;
}

KOBDEF void String_destroy(kob_String *s) {
  KOB_FREE(s->items);
  s->items = NULL;
  s->capacity = 0;
  s->count = 0;
}

KOBDEF void String_push(kob_String *s, char c) { kob_da_push(s, c); }

KOBDEF void String_push_str(kob_String *s, const char *c_str) {
  size_t len = KOB_STRLEN(c_str);
  kob_da_push_many(s, c_str, len);
}

KOBDEF kob_str String_slice(const kob_String *s, int begin, int end) {
  if (!String_isvalid(s)) {
    kob_str err1 = {0, 0, 0};
    return err1;
  }

  size_t len = String_len(s);
  const char *const *const target_ptr = (const char *const *const)&(s->items);

  if (begin >= (int)len) {
    kob_str err2 = {target_ptr, 0, 0};
    return err2;
  } else if (begin < 0) {
    begin = 0;
  }

  if (end < 0) {
    int idx = len - abs(end);
    end = idx < 0 ? 0 : idx;
  } else if (end >= (int)len) {
    end = len;
  }

  kob_str last = {target_ptr, (size_t)begin, (size_t)end - (size_t)begin};
  return last;
}

//
// str
// vv
KOBDEF kob_str str_slice(const kob_str s, int begin, int end) {
  int shifted_begin = ((int)s.start) + begin;
  int shifted_end = ((int)s.start) + end;
  kob_String *str_obj = (kob_String *)(s.ptr);
  return String_slice(str_obj, shifted_begin, shifted_end);
}

// KOBDEF uint8_t str_valid_index(const kob_str s, size_t idx) {
//   return (s.ptr != NULL && idx < s.len);
// }
// KOBDEF char str_at(const kob_str s, size_t idx) {
//   if (!str_valid_index(s, idx)) {
//     return '\0';
//   } else {
//     char c = (*(s.ptr))[idx];
//     return c;
//   }
// }

#endif // KOB_IMPL

#endif // KOB_HEADER_FILE
