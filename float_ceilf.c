#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define time_diff(start, end)                                                  \
  (end.tv_nsec - start.tv_nsec < 0                                             \
       ? (1000000000 + end.tv_nsec - start.tv_nsec)                            \
       : (end.tv_nsec - start.tv_nsec))
#define time_check(_FUNC_, cost_time)                                          \
  do {                                                                         \
    struct timespec time_start;                                                \
    struct timespec time_end;                                                  \
    double during;                                                             \
    clock_gettime(CLOCK_MONOTONIC, &time_start);                               \
    _FUNC_;                                                                    \
    clock_gettime(CLOCK_MONOTONIC, &time_end);                                 \
    during = time_diff(time_start, time_end);                                  \
    printf("%f\n", during);                                                    \
    cost_time = during;                                                        \
  } while (0)

#define FACTOR 1.5
#define CHUNK_SIZE 4
static inline float ilog_factor(float n) /* log1.5(n) = log2(n)/log2(1.5)*/
{
  return ceilf(log2f(n) / log2f(FACTOR));
}

static inline float modff_factor(float n) {
  float integer;
  modff((log2f(n) / log2f(FACTOR)), &integer);
  return integer + 1;
}

void benchmark_n(void) {
  float func = ilog_factor(10);
  printf("func %f\n", func);

  float modff_ = modff_factor(10);
  printf("modff %f\n", modff_);

  int ilog_t = 0;
  int modff_t = 0;

  for (int i = 2; i < 100000; i++) {
    double ilog_time = 0;
    double modff_time = 0;
    printf("i %d\n", i);
    time_check(ilog_factor(100), ilog_time);
    time_check(modff_factor(100), modff_time);
    printf("---------------\n");
    if (ilog_time - modff_time > 0)
      ilog_t++;
    else if (modff_time - ilog_time > 0)
      modff_t++;
  }
  printf("ceil: %d\nmodff: %d\n", ilog_t, modff_t);
}

void benchmark(int loop_size, float  (*func)(float num), const char *text_name){
  FILE *ptr = NULL;
  ptr = fopen(text_name, "a"); //w
  if (!ptr)
    return;
  struct timespec time_start;
  struct timespec time_end;
  double during;
  int time_i = 0;
  printf("start testing\n");
  for (time_i = 0; time_i < loop_size; time_i++) {
    clock_gettime(CLOCK_MONOTONIC, &time_start);
    func(4);
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    during = time_diff(time_start, time_end);
    fprintf(ptr, "%d %f\n", time_i, during);
    printf("%d finished\n", time_i);
  }
  fclose(ptr);
}


int main() {
  /*
  float func = ilog_factor(9999);
  printf("func %f\n", func);

  float modff_ = modff_factor(9999);
  printf("modff %f\n", modff_);

  float ilog_time;
  time_check(ilog_factor(9999), ilog_time);
  time_check(modff_factor(9999), ilog_time);
  */
  benchmark(1, ilog_factor, "ilog_factor.txt");
  benchmark(1, modff_factor, "modff_factor.txt");
}
