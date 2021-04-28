#ifndef __VECTOR_H_
#define __VECTOR_H_
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* vector with small buffer optimization */

#define STRUCT_BODY(type)                                                  \
    struct {                                                               \
        size_t size : 54, on_heap : 1, capacity : 6, flag1 : 1, flag2 : 1, \
            flag3 : 1;                                                     \
        type *ptr;                                                         \
    }

#define NEXT_POWER_OF_2(s) \
    !(s & (s - 1)) ? s : (size_t) 1 << (64 - __builtin_clzl(s))

#define v(t, s, name, ...)                                              \
    (void) ((struct {                                                   \
        _Static_assert(s <= 8, "it is too big");                        \
        int dummy;                                                      \
    }){1});                                                             \
    union {                                                             \
        STRUCT_BODY(t);                                                 \
        struct {                                                        \
            size_t filler;                                              \
            t buf[NEXT_POWER_OF_2(s)];                                  \
        };                                                              \
    } name __attribute__((cleanup(vec_free))) = {.buf = {__VA_ARGS__}}; \
    name.size = sizeof((__typeof__(name.buf[0])[]){0, __VA_ARGS__}) /   \
                    sizeof(name.buf[0]) -                               \
                1;                                                      \
    name.capacity = sizeof(name.buf) / sizeof(name.buf[0])

#define vec_size(v) v.size

#define FACTOR 1.5
#define CHUNK_SIZE 4
#define vec_capacity(v) \
    ((v.on_heap) ? (size_t) (CHUNK_SIZE * pow(FACTOR, v.capacity)) : sizeof(v.buf) / sizeof(v.buf[0]))

#define vec_data(v) (v.on_heap ? v.ptr : v.buf) /* always contiguous buffer */

#define vec_elemsize(v) sizeof(v.buf[0])
#define vec_pos(v, n) vec_data(v)[n] /* lvalue */

#define vec_reserve(v, n) __vec_reserve(&v, n, vec_elemsize(v), vec_capacity(v))
#define vec_push_back(v, e)                                            \
    __vec_push_back(&v, &(__typeof__(v.buf[0])[]){e}, vec_elemsize(v), \
                    vec_capacity(v))

#define vec_pop_back(v) (void) (v.size -= 1)


#define VAR_ARG_COUNT(v, ...)\
(sizeof((__typeof__(v.buf[0])[]){0, __VA_ARGS__}) /sizeof(v.buf[0]) - 1)       

#define vec_insert(v, pos, ...)\
    __vec_insert(&v, pos, vec_elemsize(v), vec_capacity(v), VAR_ARG_COUNT(v, __VA_ARGS__), &(__typeof__(v.buf[0])[VAR_ARG_COUNT(v, __VA_ARGS__)]){__VA_ARGS__})
     
#define vec_erase(v, ...)\
    __vec_erase(&v, vec_elemsize(v), vec_capacity(v), VAR_ARG_COUNT(v, __VA_ARGS__), __VA_ARGS__);

/* This function attribute specifies function parameters that are not supposed
 * to be null pointers. This enables the compiler to generate a warning on
 * encountering such a parameter.
 */
#define NON_NULL __attribute__((nonnull))

NON_NULL void vec_free(void *p);

static inline float ilog_factor(float n) /* log1.5(n) = log2(n)/log2(1.5)*/
{
    return ceilf(log2f(n)/log2f(FACTOR));
}

NON_NULL void __vec_reserve(void *vec,
                                   size_t n,
                                   size_t elemsize,
                                   size_t capacity);

NON_NULL void __vec_push_back(void *restrict vec,
                                     void *restrict e,
                                     size_t elemsize,
                                     size_t capacity);

NON_NULL void __vec_insert(void *restrict vec,
                                    size_t pos,
                                    size_t elemsize,
                                    size_t capacity,
                                    size_t arg_count,
                                    ...);

NON_NULL void __vec_erase(void *restrict vec,
                                    size_t elemsize,
                                    size_t capacity,
                                    size_t arg_count,
                                    ...);
#endif