#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "vector.h"

#define display(v)                               \
    do {                                         \
        for (size_t i = 0; i < vec_size(v); i++) \
            printf("%.2f ", vec_pos(v, i));      \
        puts(v.on_heap ? "heap" : "stack");      \
    } while (0)


void std_test(void) {
   v(float, 0, vec1);
    /*
    v(int, 2, vec2, 13, 42);

    printf("pos(vec2,0)=%d, pos(vec2,1)=%d\n", vec_pos(vec2, 0),
           vec_pos(vec2, 1));
    vec_push_back(vec2, 88);
    vec_reserve(vec2, 100);
    printf("capacity(vec1)=%zu, capacity(vec2)=%zu\n", vec_capacity(vec1),
           vec_capacity(vec2));
    printf("pos(vec2,2)=%d\n", vec_pos(vec2, 2));
    */
    display(vec1);
    vec_push_back(vec1, 0.0);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));
    display(vec1);
    vec_push_back(vec1, 1.1);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    display(vec1);
    vec_push_back(vec1, 2.2);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    display(vec1);
    vec_push_back(vec1, 3.3);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    display(vec1);
    vec_push_back(vec1, 4.4);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    display(vec1);
    vec_push_back(vec1, 5.5);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    display(vec1);
    vec_pop_back(vec1);
        printf("capacity(vec1)=%zu\n", vec_capacity(vec1));

    
    display(vec1);
    vec_pop_back(vec1);
    display(vec1);
    vec_pop_back(vec1);
    display(vec1);
    vec_pop_back(vec1);
    display(vec1);
    vec_pop_back(vec1);
    display(vec1);
    vec_pop_back(vec1);
    display(vec1);

    // vec_free(&vec1);
    display(vec1);
}

void reserve_test(void) {
    v(int, 2, vec2, 13, 42);

    //printf("pos(vec2,0)=%d, pos(vec2,1)=%d\n", vec_pos(vec2, 0),
    //    vec_pos(vec2, 1));
    
    //printf("capacity(vec2)=%zu\n", vec_capacity(vec2));
    vec_push_back(vec2, 88);
    //printf("capacity(vec2)=%zu\n", vec_capacity(vec2));    
    vec_reserve(vec2, 1024);
    //printf("capacity(vec2)=%zu\n", vec_capacity(vec2));
    //printf("pos(vec2,2)=%d\n", vec_pos(vec2, 2));
    vec_reserve(vec2, 1024);
    vec_reserve(vec2, 2048);
    vec_reserve(vec2, 4096);

}

void push_back_test(void) {
    v(int, 3, vec_int, 1, 2, 3);
    int i = 3;
    for (;i < 10;i++) {
        vec_push_back(vec_int, i);
        printf("capacity(vec1)=%zu, %d\n", vec_capacity(vec_int), vec_int.capacity);
    }
    vec_reserve(vec_int, 15);
            printf("capacity(vec1)=%zu, %d\n", vec_capacity(vec_int), vec_int.capacity);
}

void reuse_test(void) {
    v(uint64_t, 3, test, 1, 2, 3);
    printf("%10s    %10s    %10s    %10s\n", "i", "capacity", "realsize", "address");
    printf("%10d    %10d    %10ld    %10p\n", 0, test.capacity, vec_capacity(test), &vec_pos(test, 1));
    for (int i = 1;i <= 30;i++) {
        vec_reserve(test, (size_t)(CHUNK_SIZE * pow(FACTOR, i)));
        printf("%10d    %10d    %10ld    %10p\n", i, test.capacity, vec_capacity(test), &vec_pos(test, 1));
    }
}

int main()
{
    v(float, 4, test, 1.0, 2.0, 3.0, 4);
    vec_insert(test, 3, 5, 6, 7, 8, 9, 10, 11, 12);
    display(test);
    vec_erase(test, 0, 4);
    display(test);
    vec_erase(test, 7);
    display(test);
    vec_erase(test, 11);
    display(test);
    return 0;
}
