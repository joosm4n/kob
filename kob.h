#ifndef KOB_HEADER_FILE
#define KOB_HEADER_FILE

/*
 *  Flags:
 *  - KOB_IMPL: Define ONCE in your main .c/.cpp file.
 *  - KOB_HEAP: A stack based heap for primarily embedded systems.
 *  - KOB_NO_CORE: Sets the default usage flags to false so you can enable only
 * the ones you want.
 *  - KOB_NO_STD: Sets usage of cstdlib/libc to false
 *  - KOB_TYPEDEFS: Turns on custom typedefs, mainly for non cstd programming
 */

/* ----- */
/* Don't commit, just so I can see the IMPL PART when testing */
/*                                                                             \
 */                                                                            \
#define KOB_IMPL
#define KOB_HEAP /*                                                            \
 #define KOB_TYPEDEFS                                                          \
 #define KOB_NO_STD                                                            \
 #undef __cplusplus                                                            \
 */
/* Don't commit, just so I can see the IMPL PART wihen testing */
/* ----- */

#ifndef KOB_NO_STD
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* KOB_NO_STD */

#ifdef __cplusplus
#define KOB_DECLTYPE_CAST(T) (decltype(T))
#else
#define KOB_DECLTYPE_CAST(T)
#endif

#define KOBDEF
#define KOB_PTR_CAST(type, obj) ((type)(&obj))
#define KOB_BIT_CAST(type, obj) (*(type *)(&obj))

#define KOB_PASTE(x, y) KOB_PASTE_IMPL(x, y)
#define KOB_PASTE_IMPL(x, y) x##y
#define KOB_STRINGIFY(x) #x

#define KOB_STATIC_ASSERT(x, msg)                                              \
  int KOB_STATIC_ASSERT(int KOB_STATIC_ASSERT[(x) ? 1 : -1])

#ifdef KOB_TYPEDEFS
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned long long size_t;
#ifndef __cplusplus
typedef _Bool bool;
#define false (0)
#define true (1)
#endif

#endif /* KOB_TYPEDEFS */

KOB_STATIC_ASSERT(sizeof(uint8_t) == 1, "Incorrect assumed size");
KOB_STATIC_ASSERT(sizeof(uint16_t) == 2, "Incorrect assumed size");
KOB_STATIC_ASSERT(sizeof(uint32_t) == 4, "Incorrect assumed size");
KOB_STATIC_ASSERT(sizeof(uint64_t) == 8, "Incorrect assumed size");
KOB_STATIC_ASSERT(sizeof(size_t) == 8, "Incorrect assumed size");

#ifdef KOB_HEAP

#define KOB_DEFAULT_MAX_STACK_SIZE                                             \
  (1 * 1000 * 8192) /* 1 byte => 1KB => 8MiB                                   \
                     */
#define KOB_HEAP_CHUNK_SIZE (128ul)
#define KOB_HEAP_NUM_CHUNKS (10ul)
#define KOB_HEAP_SIZE (KOB_HEAP_CHUNK_SIZE * KOB_HEAP_NUM_CHUNKS)
KOB_STATIC_ASSERT(KOB_HEAP_SIZE < KOB_DEFAULT_MAX_STACK_SIZE,
                  "Reassess what your doing");

typedef struct kob_HeapChunkHeader {
  /* Incremental ID */
  uint32_t id;

  /* Size is not including size of header */
  /* This is up to change if it makes things easier */
  uint32_t size;
} kob_HeapChunkHeader;
KOB_STATIC_ASSERT(sizeof(kob_HeapChunkHeader) == (8),
                  "Unexpected size of header");

typedef struct kob_HeapChunk {
  uint8_t data[KOB_HEAP_CHUNK_SIZE];
} kob_HeapChunk;
KOB_STATIC_ASSERT(sizeof(kob_HeapChunk) == (KOB_HEAP_CHUNK_SIZE),
                  "Unexpected size of chunk");

typedef struct kob_HeapChunkMetadata {
  uint32_t id;
  uint32_t pos;
  uint32_t size;
} kob_HeapChunkMetadata;

KOBDEF void kob_free_HEAP(void *ptr);
KOBDEF void *kob_malloc_HEAP(size_t size);
KOBDEF void *kob_calloc_HEAP(size_t n, size_t size);
KOBDEF void *kob_realloc_HEAP(void *ptr, size_t size);
KOBDEF const kob_HeapChunkHeader *
kob_next_heap_header(const kob_HeapChunkHeader *header);

KOBDEF void kob_print_chunk(const kob_HeapChunkHeader *header);
KOBDEF void kob_print_heap();
KOBDEF void kob_print_heap_in_chunks();
KOBDEF void kob_print_addr(uint32_t addr);

#endif /* KOB_HEAP */

#define KOB_COUNTOF(x) ((size_t)(sizeof(x) / sizeof(x[0])))
#define KOB_ZERO(x) (KOB_MEMSET(x, 0, KOB_COUNTOF(x)))

#ifndef KOB_NO_STD
#define kob_memset memset
#define kob_strlen strlen
#define kob_memcpy memcpy
#define kob_printf(...) printf(__VA_ARGS__)
#define kob_abs abs
#define kob_assert(check, msg) assert((check && msg))
#define kob_abort(...) abort(__VA_ARGS__)
#else
#define NULL 0
#define kob_memset kob_memset_NOSTD
#define kob_memcpy kob_memcpy_NOSTD
#define kob_strlen kob_strlen_NOSTD
#define kob_printf(...) kob_printf_NOSTD(__VA_ARGS__)
#define kob_abs kob_abs_NOSTD
#define kob_assert(check, msg) kob_assert_NOSTD(check, msg)
#define kob_abort(...)

/* TEMP FIX */
int kob_printf_NOSTD(const char *format, ...) { return 0; }
void *kob_memset_NOSTD(void *s, int c, size_t n) { return NULL; }
size_t kob_strlen_NOSTD(const char *s) { return 0; }
int kob_abs_NOSTD(int i) { return 0; }
void *kob_memcpy_NOSTD(void *dest, const void *src, size_t n) { return NULL; }
void kob_assert_NOSTD(bool expression_result, const char *exprsesion_string) {
  if (expression_result == false) {
    kob_printf("Oh no: expression %s failed!\n", exprsesion_string);
    kob_abort();
  }
}

#endif /* KOB_NO_STD */

#ifndef KOB_HEAP
#define kob_free free
#define koc_malloc malloc
#define kob_calloc calloc
#define kob_realloc realloc
#else
#define kob_free kob_free_HEAP
#define kob_malloc kob_malloc_HEAP
#define kob_calloc kob_calloc_HEAP
#define kob_realloc kob_realloc_HEAP
#endif /* KOB_HEAP */

#define KOB_FOREACH(_item_, _array_)                                           \
  for (size_t i = 0u, _keep_ = 1u; _keep_ && i < KOB_COUNTOF(_array_);         \
       _keep_ = !_keep_, i++)                                                  \
    for (_item_ = _array_[i]; _keep_; _keep_ = !_keep_)

#define KOB_LOG_INFO(fmt, ...)                                                 \
  {                                                                            \
    kob_printf("[KOB-INFO]: ");                                                \
    kob_printf(fmt, ##__VA_ARGS__);                                            \
    kob_printf("\n");                                                          \
  }

#define KOB_LOG_ERROR(fmt, ...)                                                \
  {                                                                            \
    kob_printf("[KOB-ERROR] : ");                                              \
    kob_printf(fmt, ##__VA_ARGS__);                                            \
    kob_printf("\n");                                                          \
  }

#define KOB_LOG_ERROR2(fmt, ...)                                               \
  {                                                                            \
    kob_printf("[KOB-ERROR] : %s:%d \n          ERROR: ", __FILE__, __LINE__); \
    kob_printf(fmt, ##__VA_ARGS__);                                            \
    kob_printf("\n");                                                          \
  }

#define KOB_ASSERT(check, msg, ...)                                            \
  if (!(check)) {                                                              \
    kob_printf("[KOB-ASSERT] %s:%d: ", __FILE__, __LINE__);                    \
    kob_printf(msg, ##__VA_ARGS__);                                            \
    kob_printf("\n");                                                          \
    kob_assert(check, msg);                                                    \
  }

#define TODO(msg)                                                              \
  {                                                                            \
    kob_printf("[KOB-TODO] %s:%d: ", __FILE__, __LINE__);                      \
    kob_printf(msg, ##__VA_ARGS__);                                            \
    kob_printf("\n");                                                          \
    kob_assert(false, msg);                                                    \
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
          kob_realloc((da)->items, (da)->capacity * sizeof(*(da)->items));     \
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
    kob_memcpy((da)->items + (da)->count, (new_items),                         \
               (new_items_count) * sizeof(*(da)->items));                      \
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

kob_String String_from(const char *c_str);
void String_destroy(kob_String *s);
void String_push(kob_String *s, char c);
kob_str String_slice(const kob_String *s, int begin, int end);
void String_push_str(kob_String *s, const char *c_str);
#define String_isvalid(s) ((s) != NULL ? (s)->items != NULL : 0)
#define String_len(s) ((s)->count)
#define String_first(s) (kob_da_first(s))
#define String_last(s) (kob_da_last(s))

typedef struct kob_str {
  const char *const *const ptr;
  const size_t start;
  const size_t len;
} kob_str;

kob_str str_slice(const kob_str s, int begin, int end);
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

#endif /* __cplusplus */

#ifdef KOB_IMPL

/* */
/* String */
/* vv */
KOBDEF kob_String String_from(const char *c_str) {
  kob_String s = {};
  kob_String *s_ref = &s;
  size_t len = kob_strlen(c_str);
  kob_da_push_many(s_ref, c_str, len);
  if (kob_da_last(s_ref) != '\0') {
    kob_da_push(s_ref, '\0');
  }
  return s;
}

KOBDEF void String_destroy(kob_String *s) {
  kob_free(s->items);
  s->items = NULL;
  s->capacity = 0;
  s->count = 0;
}

KOBDEF void String_push(kob_String *s, char c) { kob_da_push(s, c); }

KOBDEF void String_push_str(kob_String *s, const char *c_str) {
  size_t len = kob_strlen(c_str);
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
    int idx = len - kob_abs(end);
    end = idx < 0 ? 0 : idx;
  } else if (end >= (int)len) {
    end = len;
  }

  kob_str last = {target_ptr, (size_t)begin, (size_t)end - (size_t)begin};
  return last;
}

/* */
/* str */
/* vv */
KOBDEF kob_str str_slice(const kob_str s, int begin, int end) {
  int shifted_begin = ((int)s.start) + begin;
  int shifted_end = ((int)s.start) + end;
  kob_String *str_obj = (kob_String *)(s.ptr);
  return String_slice(str_obj, shifted_begin, shifted_end);
}

#ifdef KOB_HEAP

static uint8_t KOB_HEAP_ARRAY[KOB_HEAP_SIZE];
static uint8_t *const KOB_HEAP_BASE_PTR = KOB_HEAP_ARRAY;
static uint8_t *KOB_HEAP_CURR_PTR = KOB_HEAP_ARRAY;
static uint32_t KOB_HEAP_ID_COUNTER = 0;

void kob_free_HEAP(void *ptr) {}

void *kob_malloc_HEAP(size_t size) {
  if (KOB_HEAP_BASE_PTR + size >= KOB_HEAP_BASE_PTR + KOB_HEAP_SIZE) {
    return NULL;
  }
  kob_HeapChunkHeader *chunk_header = (kob_HeapChunkHeader *)KOB_HEAP_CURR_PTR;
  kob_HeapChunkHeader new_header;
  new_header.id = KOB_HEAP_ID_COUNTER;
  new_header.size = size;
  (*chunk_header) = new_header;

  void *ret_ptr = KOB_HEAP_CURR_PTR + sizeof(kob_HeapChunkHeader);
  const uint8_t *const next_not_aligned = KOB_HEAP_CURR_PTR + size;
  while (KOB_HEAP_CURR_PTR < next_not_aligned) {
    KOB_HEAP_CURR_PTR += KOB_HEAP_CHUNK_SIZE;
    KOB_HEAP_ID_COUNTER += KOB_HEAP_CHUNK_SIZE;
  }
  return ret_ptr;
}

void *kob_calloc_HEAP(size_t n, size_t size) {
  void *ptr = kob_malloc_HEAP(size);
  if (ptr != NULL) {
    kob_memset(ptr, n, size);
  }
  return ptr;
}

void *kob_realloc_HEAP(void *ptr, size_t size) { return kob_malloc(size); }

const kob_HeapChunkHeader *
kob_next_heap_header(const kob_HeapChunkHeader *header) {

  kob_HeapChunk *chunk = (kob_HeapChunk *)header;
  const uint8_t *const next_not_aligned =
      ((uint8_t *)chunk) + sizeof(kob_HeapChunkHeader) + header->size;

  while ((uint8_t *)chunk < next_not_aligned) {
    chunk += 1;
  }

  return (kob_HeapChunkHeader *)chunk;
}

#define KOB_HEAP_PRINT_4BYTES(_print_bytes)                                    \
  {                                                                            \
    printf("%02x%02x %02x%02x - %c%c%c%c\n", *((_print_bytes)),                \
           *((_print_bytes) + 1), *((_print_bytes) + 2),                       \
           *((_print_bytes) + 3), *((_print_bytes)), *((_print_bytes) + 1),    \
           *((_print_bytes) + 2), *((_print_bytes) + 3));                      \
  }

void kob_print_chunk(const kob_HeapChunkHeader *header) {
  printf("Chunk #%u[%u]\n", header->id, header->size);

  const uint8_t *const bytes = (uint8_t *)(header);
  printf(" CHUNK_ID - ");
  KOB_HEAP_PRINT_4BYTES(bytes);
  printf(" CHUNK_SZ - ");
  KOB_HEAP_PRINT_4BYTES(bytes + 4);

  uint32_t i = 8;
  while (i + 4 <= header->size) {
    printf(" %08X - ", header->id + i);
    KOB_HEAP_PRINT_4BYTES(bytes + i);
    i += 4;
  }

  printf("\n");
}

void kob_print_heap() {
  uint32_t heap_size = KOB_HEAP_CURR_PTR - KOB_HEAP_BASE_PTR;
  uint32_t i = 0;

  printf(" --- KOB_HEAP: Start ---\n");
  while (i <= heap_size) {
    printf(" %08X - ", i);
    uint8_t *bytes = KOB_HEAP_ARRAY + i;
    KOB_HEAP_PRINT_4BYTES(bytes);
    i += 4;
  }

  printf(" --- KOB_HEAP: End ---\n");
  printf("\n\t---- Stats ---- \n\tTOTAL_ALLOCAED_HEAP_SIZE = %ld (bytes)\n\t "
         "USED_HEAP_SIZE = %d "
         "(bytes)\n\n",
         KOB_HEAP_SIZE, heap_size);
}

void kob_print_heap_in_chunks() {
  const kob_HeapChunkHeader *header = (kob_HeapChunkHeader *)KOB_HEAP_BASE_PTR;
  do {
    kob_print_chunk(header);
    header = kob_next_heap_header(header);
  } while (header->id != 0);
}

void kob_print_addr(uint32_t addr) {
  if (addr < KOB_HEAP_SIZE) {
    printf(" Bytes at %08X - ", addr);
    uint8_t *bytes = KOB_HEAP_BASE_PTR + addr;
    KOB_HEAP_PRINT_4BYTES(bytes);
  }
}
#endif /* KOB_HEAP */

#endif /* KOB_IMPL */

#endif /* KOB_HEADER_FILE */
