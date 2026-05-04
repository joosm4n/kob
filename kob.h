#ifndef KOB_HEADER_FILE
#define KOB_HEADER_FILE

#define KOB_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#define KOB_DECLTYPE_CAST(T) (decltype(T))
#else
#define KOB_DECLTYPE_CAST(T)
#endif

#define KOB_PTR_CAST(type, obj) ((type)(&obj))
#define KOB_BIT_CAST(type, obj) (*(type *)(&obj))

#define KOB_PASTE(x) #x

#define KOB_COUNTOF(x) (sizeof(x) / sizeof(x[0]))

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
    assert((check) && (msg));                                                  \
  }

#define KOB_REALLOC realloc

typedef struct _kob_dynarray_header {
  size_t count;
  size_t capacity;
  void *data;
} KOB_DA_HEADER;

#define kob_da_reserve(da, expected_capacity)                                  \
  do {                                                                         \
    if ((expected_capacity) > (da)->capacity) {                                \
      if ((da)->capacity == 0) {                                               \
        (da)->capacity = NOB_DA_INIT_CAP;                                      \
      }                                                                        \
      while ((expected_capacity) > (da)->capacity) {                           \
        (da)->capacity *= 2;                                                   \
      }                                                                        \
      (da)->items = KOB_DECLTYPE_CAST((da)->items)                             \
          KOB_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));     \
      KOB_ASSERT((da)->items != NULL && "Buy more RAM lol");                   \
    }                                                                          \
  } while (0)

#define kob_da_append(da, item)                                                \
  do {                                                                         \
    KOB_DA_RESERVE((da), (da)->count + 1);                                     \
    (da)->items[(da)->count++] = (item);                                       \
  } while (0)

#define KOB_DA_APPEND_MANY(da, new_items, new_items_count)                     \
  do {                                                                         \
    KOB_DA_RESERVE((da), (da)->count + (new_items_count));                     \
    memcpy((da)->items + (da)->count, (new_items),                             \
           (new_items_count) * sizeof(*(da)->items));                          \
    (da)->count += (new_items_count);                                          \
  } while (0)

#define KOB_DA_RESIZE(da, new_size)                                            \
  do {                                                                         \
    KOB_DA_RESERVE((da), new_size);                                            \
    (da)->count = (new_size);                                                  \
  } while (0)

#define kob_da_pop(da) (da)->items[(KOB_ASSERT((da)->count > 0), --(da)->count)]
#define kob_da_first(da) (da)->items[(KOB_ASSERT((da)->count > 0), 0)]
#define kob_da_last(da)                                                        \
  (da)->items[(KOB_ASSERT((da)->count > 0), (da)->count - 1)]
#define kob_da_remove_unordered(da, i)                                         \
  do {                                                                         \
    size_t j = (i);                                                            \
    NOB_ASSERT(j < (da)->count);                                               \
    (da)->items[j] = (da)->items[--(da)->count];                               \
  } while (0)

#ifdef __cplusplus

#define KOB_RULE_OF_FIVE(name)                                                 \
  KOB_PASTE(name)() = default;                                                 \
  KOB_PASTE(name)(const KOB_PASTE(name) &) = delete;                           \
  KOB_PASTE(name)(KOB_PASTE(name) &&) = delete;                                \
  KOB_PASTE(name) &operator=(const KOB_PASTE(name) &) = delete;                \
  KOB_PASTE(name) &operator=(KOB_PASTE(name) &&) = delete;                     \
  virtual ~KOB_PASTE(name)() = default;

#endif // __cplusplus

#endif // KOB_HEADER_FILE
