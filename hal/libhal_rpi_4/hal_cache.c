/*
 * Copyright 2023  Marven Gilhespie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include <machine/cheviot_hal.h>
#include <sys/param.h>

/*
 *
 */
uint32_t hal_cache_line_size(void)
{
  uint32_t ccsidr;
  uint32_t shift;

  ccsidr = hal_get_ccsidr();
  shift = (ccsidr & 0x07) + 4;
  return (1 << shift);
}


/* @brief   Invalidate the current CPU's data cache
 *
 * Invalidates the cache content that are between addresses
 * start_vaddr and end_vaddr.
 *
 * Call this after a DMA operation writes data to main memory
 * so that the CPU does not pick up stale data. Performs the DCIMVAC
 * operation on this memory range.
 */
void hal_invalidate_dcache(void *start_vaddr, void *end_vaddr)
{
  uint32_t line_size;
  uintptr_t va = (uintptr_t)start_vaddr;
  uintptr_t end = (uintptr_t)end_vaddr;

  line_size = hal_cache_line_size();
  va = rounddown(va, line_size);
  hal_dsb();

  while (va < end) {
    hal_cache_dcimvac(va);
    va += line_size;
  }

  hal_dsb();
  hal_isb();
}


/* @brief   Clean the data cache
 *
 * Cleans the cache content that are between addresses
 * start_vaddr and end_vaddr. Performs the DCCMVAC operation
 * on this memory range.
 */
void hal_clean_dcache(void *start_vaddr, void *end_vaddr)
{
  uintptr_t va = (uintptr_t)start_vaddr;
  uintptr_t end = (uintptr_t)end_vaddr;
  uint32_t line_size;

  line_size = hal_cache_line_size();
  va = rounddown(va, line_size);
  hal_dsb();

  while (va < end) {
    hal_cache_dccmvac(va);
    va += line_size;
  }

  hal_dsb();
  hal_isb();
}


/* @brief   Flush the data cache
 *
 * Flushes the cache content that are between addresses
 * start_vaddr and end_vaddr.
 *
 * Call this prior to a DMA operation that reads data so that
 * data is written to main memory before a DMA device reads
 * from it. Performs the DCCIMVAC operation on this memory range.
 */
void hal_flush_dcache(void *start_vaddr, void *end_vaddr)
{
  uintptr_t va = (uintptr_t)start_vaddr;
  uintptr_t end = (uintptr_t)end_vaddr;
  uint32_t line_size;

  line_size = hal_cache_line_size();
  va = rounddown(va, line_size);
  hal_dsb();

  while (va < end) {
    hal_cache_dccimvac(va);
    va += line_size;
  }

  hal_dsb();
  hal_isb();
}


