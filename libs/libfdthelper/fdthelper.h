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

#ifndef SYS_FDTHELPER_H
#define SYS_FDTHELPER_H

#include <stdint.h>


/*
 *
 */
struct fdthelper
{
  int fd;
  ssize_t fdt_size;
  void *fdt;
};


/*
 * Prototypes
 */
int load_fdt(char *path, struct fdthelper *helper);
int unload_fdt(struct fdthelper *helper);

int fdthelper_check_compat(const void *fdt, int offset, char *req_compat);
int fdthelper_translate_address(const void *fdt, void *addr, void **raddr);
int fdthelper_get_reg(const void *fdt, int offset, void **raddr, int *rsize);
int fdthelper_get_irq(const void *fdt, int offset, int *rirq);

#endif
