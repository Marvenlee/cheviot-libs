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
 *
 * *************************************************************************
 * 
 * Simple profiling library for measuring performance of sections of code
 * 
 * TODO: Extend API, allow dynamic sizing of profiling buffers
 */

#define LOG_LEVEL_ERROR

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/profiling.h>
#include <sys/syscalls.h>
#include <sys/debug.h>


/*
 *
 */
void profiling_init_samples(struct profiling_samples *ps)
{
  memset(ps, 0, sizeof *ps);
}


/*
 *
 */
void profiling_add_sample(struct profiling_samples *ps, int val)
{
  if (ps->sample_cnt < MAX_PROFILING_SAMPLES) {
    ps->window[ps->i] = val;

    ps->i = (ps->i + 1) % MAX_PROFILING_SAMPLES;
    ps->sample_cnt++;

    ps->sum += (int64_t)val;  
    ps->avg = (int)(ps->sum / ps->sample_cnt);
  } else {
    int old_val = ps->window[ps->i];
    ps->window[ps->i] = val;
    
    ps->i = (ps->i + 1) % MAX_PROFILING_SAMPLES;

    ps->sum -= (int64_t)old_val;
    ps->sum += (int64_t)val;    
    ps->avg = (int)(ps->sum / ps->sample_cnt);
  }

  if (ps->sample_cnt == 1) {
    ps->min = val;
    ps->max = val;
  } else if (val < ps->min) {
    ps->min = val;
  } else if (val > ps->max) {
    ps->max = val;
  }  
}


/*
 *
 */
void profiling_microsecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end)
{
  struct timespec diff;
  int micros;
  
  diff_timespec(&diff, end, start);
  micros = (int)(((uint64_t)diff.tv_sec * 1000000) + diff.tv_nsec / 1000);  
  profiling_add_sample(ps, micros);
}


/*
 *
 */
void profiling_millisecs(struct profiling_samples *ps, struct timespec *start, struct timespec *end)
{
  struct timespec diff;
  int millis;
  
  diff_timespec(&diff, end, start);    
  millis = (int)(((uint64_t)diff.tv_sec * 1000) + diff.tv_nsec / 1000000);
  profiling_add_sample(ps, millis);
}

