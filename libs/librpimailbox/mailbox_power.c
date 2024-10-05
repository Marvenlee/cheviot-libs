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
#include <errno.h>
#include <machine/cheviot_hal.h>
#include <sys/syscalls.h>
#include "sys/rpi_mailbox.h"

int rpi_mailbox_get_power_state(uint32_t device_id, uint32_t *state)
{
	int sc;
  struct mailbox_req req;
  struct mailbox_resp rep;
  msgiov_t siov[1] = {{ .addr = &req, .size = sizeof req}};
  msgiov_t riov[1] = {{ .addr = &rep, .size = sizeof rep}};

  req.cmd = MBOX_TAG_GET_POWER_STATE;
  req.u.get_power_state.device_id = device_id;
  
  sc = sendmsg(_mailbox_fd, MSG_SUBCLASS_RPIMAILBOX, 1, siov, 1, riov);
  
	if (sc != 0) {
		return -1;
	}
	
	*state = rep.u.get_power_state.state;
	return 0;
}


int rpi_mailbox_set_power_state(uint32_t device_id, uint32_t state)
{	
	int sc;
  struct mailbox_req req;
  msgiov_t siov[1] = {{ .addr = &req, .size = sizeof req}};

  req.cmd = MBOX_TAG_SET_POWER_STATE;
  req.u.set_power_state.device_id = device_id;
  req.u.set_power_state.state = state;
  
  sc = sendmsg(_mailbox_fd, MSG_SUBCLASS_RPIMAILBOX, 1, siov, 0, NULL);
  
	if (sc != 0) {
		return -1;
	}
	return 0;
}

