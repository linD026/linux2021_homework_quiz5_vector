#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "vector.h"

NON_NULL void vec_free(void *p)
{
    STRUCT_BODY(void) *s = p;
    if (s->on_heap)
        free(s->ptr);
}

NON_NULL void __vec_reserve(void *vec,
                                   size_t n,
                                   size_t elemsize,
                                   size_t capacity)
{
    union {
        STRUCT_BODY(void);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;

    if (n > capacity) {
        float new_cap = ilog_factor(n/CHUNK_SIZE);
        new_cap = (new_cap < 1) ? 1 : new_cap;
        if (v->on_heap) {
            v->ptr = realloc(v->ptr,
                             elemsize * (size_t)(CHUNK_SIZE * pow(FACTOR, v->capacity = new_cap)));
        } else {
            void *tmp =
                malloc(elemsize * (size_t)(CHUNK_SIZE * pow(FACTOR, v->capacity = new_cap)));
            memcpy(tmp, v->buf, elemsize * v->size);
            v->ptr = tmp;
            v->on_heap = 1;
        }
    }
}

NON_NULL void __vec_push_back(void *restrict vec,
                                     void *restrict e,
                                     size_t elemsize,
                                     size_t capacity)
{
    union {
        STRUCT_BODY(char);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;

    if (v->on_heap) {
        if (v->size == capacity)
            v->ptr = realloc(v->ptr, elemsize * CHUNK_SIZE * pow(FACTOR, (++v->capacity)));
        memcpy(&v->ptr[v->size++ * elemsize], e, elemsize);
    } else {
        if (v->size == capacity) {
            v->capacity = 1;
            void *tmp =
                malloc(elemsize * (size_t)(CHUNK_SIZE * pow(FACTOR, (v->capacity))));
            memcpy(tmp, v->buf, elemsize * v->size);
            v->ptr = tmp;
            v->on_heap = 1;
            memcpy(&v->ptr[v->size++ * elemsize], e, elemsize);
        } else
            memcpy(&v->buf[v->size++ * elemsize], e, elemsize);
    }
}

NON_NULL void __vec_insert(void *restrict vec,
                                    size_t pos,
                                    size_t elemsize,
                                    size_t capacity,
                                    size_t arg_count,
                                    ...)
{
    union {
        STRUCT_BODY(char);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;  
    //printf("%ld\n", arg_count);

    if (pos >= 0 && pos <= v->size) {
        va_list args;
        va_start(args, arg_count);
        char *e_list = (char *)va_arg(args, void *restrict);
        va_end(args);
        // 0 1 2 3 insert 2 , 3 - 2 + 1 = 2 
        // 0 1 - 3 4 

        // printf("size %ld arg_count %ld capacity %ld\n", (size_t)v->size, arg_count, capacity);
        while (v->size + arg_count >= capacity) {
            if (!v->on_heap) {
                v->capacity = 1;
                void *tmp =
                    malloc(elemsize * (size_t)(CHUNK_SIZE * pow(FACTOR, (v->capacity))));
                memcpy(tmp, v->buf, elemsize * v->size);
                v->ptr = tmp;
                v->on_heap = 1;
            }
            else {
                v->ptr = realloc(v->ptr, elemsize * CHUNK_SIZE * pow(FACTOR, (++v->capacity)));
            }
            capacity = (size_t) (CHUNK_SIZE * pow(FACTOR, v->capacity));
            
            // printf("size %ld arg_count %ld capacity %ld\n", (size_t)v->size, arg_count, capacity);
        }

        char *ptr = NULL;
        if (v->on_heap) {
            if (v->size + arg_count >= capacity) 
                v->ptr = realloc(v->ptr, elemsize * CHUNK_SIZE * pow(FACTOR, (++v->capacity)));
            ptr = v->ptr;
        }
        else
            ptr = v->buf;
            
        size_t after_pos = pos;
        char *after_pos_ptr = (char *) &ptr[after_pos * elemsize];
        void *after_pos_buf = malloc(elemsize * (size_t)(v->size - pos));
        memcpy(after_pos_buf, after_pos_ptr, elemsize * (size_t)(v->size - pos));

        for (int i = 0;i < arg_count;i++) {
            memcpy(&ptr[after_pos++ * elemsize], &e_list[i * elemsize], elemsize);
        }
        //printf("%d\n", after_pos);
        memcpy(&ptr[after_pos * elemsize], after_pos_buf, elemsize * ((size_t)v->size - pos));
        free(after_pos_buf);
        v->size = v->size + arg_count;
    }
}

NON_NULL void __vec_erase(void *restrict vec,
                                    size_t elemsize,
                                    size_t capacity,
                                    size_t arg_count,
                                    ...)
{
    union {
        STRUCT_BODY(char);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;  

    va_list args;
    va_start(args, arg_count);
    
    char *ptr = NULL;
    if (v->on_heap)
        ptr = v->ptr;
    else
        ptr = v->buf;
    
    if (arg_count > 1) {
        size_t start = va_arg(args, size_t);
        size_t end = va_arg(args, size_t);
        if (start >= 0 && end <= v->size && v->size - (end - start) > 0) {
            memmove(&ptr[elemsize * start], &ptr[elemsize * (end)], elemsize * ((size_t)(v->size - (end - start))));
            v->size = v->size - (end - start);
        }
    }
    else {
        size_t pos = va_arg(args, size_t);
        if (pos >= 0 && pos <= v->size && v->size > 0) {
            memmove(&ptr[elemsize * pos], &ptr[elemsize * (pos + 1)], elemsize * ((size_t)(v->size - pos)));
            v->size--;
        }
    }
    va_end(args);
} 