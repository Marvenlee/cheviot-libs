/*
 * Copyright 2023  Marven Gilhespie
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <libfdt.h>
#include <string.h>
#include <unistd.h>
#include <sys/debug.h>
#include <sys/stat.h>
#include "fdthelper.h"


// Internal structures
struct fdt_range
{
  uint32_t src_addr;
  uint32_t src_addrh;
  uint32_t dst_addr;
  uint32_t size;
};


/* @brief   Load an FDT
 *
 */
int load_fdt(char *path, struct fdthelper *helper)
{
  struct stat st;
  ssize_t nbytes_read;

  memset(helper, 0, sizeof *helper);

  helper->fd = open(path, O_RDONLY);

  if (helper->fd == -1) {
    return -1;
  }

  if (fstat(helper->fd, &st) != 0) {
    close(helper->fd);
    return -1;
  }

  helper->fdt_size = st.st_size;

  if (helper->fdt_size == 0) {
    close(helper->fd);
    return -1;
  }


  helper->fdt = malloc(helper->fdt_size);

  if (helper->fdt == NULL) {
    close(helper->fd);
    return -1;
  }

  nbytes_read = read(helper->fd, helper->fdt, helper->fdt_size);

  if (nbytes_read != helper->fdt_size) {
    free(helper->fdt);
    close(helper->fd);
    return -1;
  }

  return 0;
}


/* @brief   Free resources allocated by the load_fdt function
 *
 */
int unload_fdt(struct fdthelper *helper)
{
  free(helper->fdt);
  return 0;
}


/*
 *
 */
int fdt_get_range(const void *prop, int index, struct fdt_range *range)
{
  range->src_addr  = fdt32_to_cpu(((fdt32_t *)prop)[index*4 + 0]);  // use fdt64
  range->src_addrh = fdt32_to_cpu(((fdt32_t *)prop)[index*4 + 1]);
  range->dst_addr  = fdt32_to_cpu(((fdt32_t *)prop)[index*4 + 2]);
  range->size      = fdt32_to_cpu(((fdt32_t *)prop)[index*4 + 3]);
} 


/* @brief   Translate an address from a peripheral address space to the cpu space
 *
 * This is quick and dirty.  We should handle #address-cells to determine sizes
 * of fields and ascend the device tree to the top level, translating at any
 * level that contains a ranges section.
 */
int fdthelper_translate_address(const void *fdt, void *addr, void **raddr)
{
  int len;
  struct fdt_range range;
  uintptr_t taddr;

  int offset = fdt_path_offset(fdt, "/soc");
  if (offset < 0) {
    return -1;
  }

  const void *prop = fdt_getprop(fdt, offset, "ranges", &len);
  if (prop == NULL) {
    return -1;
  }

  taddr = (uintptr_t)addr;

  for (int i = 0; i < len / (4 * sizeof(uint32_t)); i++) {
    fdt_get_range(prop, i, &range);

    if (range.src_addr <= taddr && range.src_addr + range.size > taddr) {
      offset = taddr - range.src_addr;
      taddr = range.dst_addr + offset;
      *raddr = (void *)taddr;
      return 0;
    }
  }  

  return -1;
}


/* @brief   Check the compatible field of a node.
 *
 * This is quick and dirty.  We need to handle multiple compatible strings.
 */
int fdthelper_check_compat(const void *fdt, int offset, char *req_compat)
{
  int len;
  
  const char *prop = fdt_getprop(fdt, offset, "compatible", &len);
  if (prop == NULL) {
    return -1;
  }

  if (strcmp(prop, req_compat) != 0) {
    return -1;
  }

  return 0;
}


/* @brief   Get the base address of a node's registers and its size
 *
 * This is quick and dirty.  Gets the first register base address and size.
 * This needs to handle multiple register ranges.
 */
int fdthelper_get_reg(const void *fdt, int offset, void **raddr, int *rsize)
{
  int len;

  const void *prop = fdt_getprop(fdt, offset, "reg", &len);
  if (prop == NULL) {
    return -1;
  }
  // print the reg address and size
  len = len / sizeof(uint32_t);

  if (len < 2) {
    return -1;
  } 

  *raddr = (void *)fdt32_to_cpu(((fdt32_t *)prop)[0]);
  *rsize = (int)fdt32_to_cpu(((fdt32_t *)prop)[1]) * sizeof(uint32_t);

  return 0;
}  
 

/* @brief   Read the interrupts field of a node in the device tree
 *
 * This is quick and dirty. It assumes a fixed tuple with 2 entries,
 * the second of which is the IRQ.  We need a way to determine the
 * format of the tuple, what interrupt controller it's for and
 * handle multiple interrupts.
 * 
 * The kernel add_interrupt_server syscall handles the offset of the
 * base of the IRQ tables.
 */
int fdthelper_get_irq(const void *fdt, int offset, int *rirq)
{
  int len;
  uint32_t intflags;

  const void *prop = fdt_getprop(fdt, offset, "interrupts", &len);
  if (prop == NULL) {
    return -1;
  }

  len = len / sizeof(uint32_t);

  //   if (len < 2) {
  //      return -1;
  //   } 

  intflags = (uint32_t)fdt32_to_cpu(((fdt32_t *)prop)[0]);
  *rirq = (int)fdt32_to_cpu(((fdt32_t *)prop)[1]);

  return 0;
}  


