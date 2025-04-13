
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
#include <sys/syscalls.h>
#include "sys/sysinit.h"
#include <errno.h>


int sysinit_shutdown(int how)
{
	int sc;
	int fd;
  struct sysinit_req req;
  msgiov_t siov[1] = {{ .addr = &req, .size = sizeof req}};

  fd = open(SYSINIT_PATH, O_RDWR);
  
  if (fd < 0) {
    return -1;
  }

  req.cmd = SYSINIT_SHUTDOWN;
  req.u.shutdown.how = how;
  
  sc = sendio(fd, MSG_SUBCLASS_SYSINIT, 1, siov, 0, NULL);

	close(fd);
  
	if (sc != 0) {
		return -1;
	}
	  
  return 0;
}

