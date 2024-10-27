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


#define MAX_PROFILING_SAMPLES 128


/* @brief   Structure for recording profiling samples and statistics
 */
struct profiling_samples
{
  int window[MAX_PROFILING_SAMPLES];
  int sample_cnt;
  int i;
  int avg;
  int max;
  int min;
  int64_t sum;
};



void profiling_init_samples(struct profiling_samples *ps);
void profiling_add_sample(struct profiling_samples *ps, int val);
void profiling_microsecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end);
void profiling_millisecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end);



#endif

