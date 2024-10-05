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

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <machine/cheviot_hal.h>
#include "sys/rpi_mailbox.h"
#include <errno.h>


// globals
int _mailbox_fd = -1;


/*
 *
 */
int init_mailbox(void)
{
  _mailbox_fd = open("/dev/mailbox", O_RDWR);
  
  if (_mailbox_fd < 0) {
    return -1;
  }
  
  return 0;
}


/*
 *
 */
void fini_mailbox(void)
{
  close(_mailbox_fd);
}


