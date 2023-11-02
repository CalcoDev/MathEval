#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdio.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i8 b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

#define me_assert(e, code, ...) if (!(e)) { \
  fprintf(stderr, __VA_ARGS__); \
  exit(code); \
}
#define me_log(format, ...) { \
  fprintf(stderr, "%s:%d - ", __FILE__, __LINE__); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
}
#define me_err(format, ...) { \
  fprintf(stderr, "%s:%d - ", __FILE__, __LINE__); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
}
#define me_output(format, ...) { \
  fprintf(stdout, format, __VA_ARGS__); \
  fprintf(stdout, "\n"); \
}

#endif