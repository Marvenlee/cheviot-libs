/*
 * Copyright 2014  Marven Gilhespie
 *
 * Licensed under the Apache License, segment_id 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYS_PROFILING_H
#define SYS_PROFILING_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/lists.h>
#include <sys/stat.h>




/* @brief   Structure for recording profiling samples and statistics
 */
struct profiling_samples
{
  struct timespec start_ts;
  struct timespec end_ts;
  int *window;          // [MAX_PROFILING_SAMPLES];
  int window_size;
  int sample_cnt;
  int i;
  int64_t sum;
  int avg;
  int max;
  int min;
};



void profiling_init_samples(struct profiling_samples *ps);
void profiling_add_sample(struct profiling_samples *ps, int val);
void profiling_microsecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end);
void profiling_millisecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end);


#define profiling_enable(enable)                                                  \
  __libprofiling_enable = enable

// Macro for defining a structure and array to record profiling times
#define profiling_define_ts(varname, sz)                                          \
    int profiling_window_ ## varname[sz];                                         \
    struct profiling_samples profiling_ts_ ## varname = {                         \
      .window = &profiling_window_ ## varname,                                    \
      .window_size = sz,                                                          \
      .sample_cnt = 0,                                                            \
      .i = 0                                                                      \
    }
          
#define profiling_extern_ts(varname);                                             \
  extern struct profiling_samples profiling_ts_ ## varname;
  
// Macro for defining a profiling counter  
#define profiling_define_counter(varname)                                         \
  int profiling_counter_ ## varname;

#define profiling_extern_counter(varname);                                        \
  extern int profiling_counter_ ## varname;


// Macros for recording the start and end of a section to profile
#define profiling_begin(varname)                                                  \
  if (__libprofiling_enable) {                                                    \
    clock_gettime(CLOCK_MONOTONIC_RAW, &profiling_ts_ ## varname.start_ts);       \
  }   

#define profiling_end_usec(varname)                                               \
  if (__libprofiling_enable) {                                                    \
    clock_gettime(CLOCK_MONOTONIC_RAW, &profiling_ts_ ## varname.end_ts);         \
    profiling_microsecs(&profiling_ts_ ## varname ,                               \
              &profiling_ts_ ## varname.start_ts,                                 \
              &profiling_ts_ ## varname.end_ts);                                  \
  }

#define profiling_end_msec(varname)                                               \
  if (__libprofiling_enable) {                                                    \
    clock_gettime(CLOCK_MONOTONIC_RAW, &profiling_ts_ ## varname.end_ts);         \
    profiling_millisecs(&profiling_ts_ ## varname ,                               \
              &profiling_ts_ ## varname.start_ts,                                 \
              &profiling_ts_ ## varname.end_ts);                                  \
  }


// Macros to retrieve last values  
#define profiling_ts_avg(varname)                                                 \
  profiling_ts_ ## varname.avg
        
#define profiling_ts_min(varname)                                                 \
  profiling_ts_ ## varname.min

#define profiling_ts_max(varname)                                                 \
  profiling_ts_ ## varname.max

// Macro to reset profiling times and sample window    
#define profiling_ts_reset(varname)                                               \
  if (1) {                                                                        \
    profiling_ts_ ## varname.sample_cnt = 0;                                      \
    profiling_ts_ ## varname.i = 0;                                               \
  } 
    
 
// Macro for incrementing a profiling counter
#define profiling_count(varname)                                                  \
  if (__libprofiling_enable) {                                                    \
    profiling_counter_ ## varname ++;                                             \
  }

#define profiling_count_get(varname)                                              \
  profiling_counter_ ## varname

// Macro to reset profiling counter
#define profiling_count_reset(varname)                                            \
  profiling_counter_ ## varname = 0;


// Variable to control profiling
extern int __libprofiling_enable;



#endif

