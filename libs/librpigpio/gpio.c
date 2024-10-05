
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
#include <sys/syscalls.h>
#include <sys/rpi_gpio.h>
#include <errno.h>


// Statics
static int _gpio_fd = -1;


/*
 *
 */
int init_gpio(void)
{
  _gpio_fd = open("/dev/gpio", O_RDWR);
  
  if (_gpio_fd < 0) {
    return -1;
  }
  
  return 0;
}


/*
 *
 */
void fini_gpio(void)
{
  close(_gpio_fd);
}


/*
 *
 */
int set_gpio(int gpio, int state)
{
  struct msg_gpio_req header;
  msgiov_t siov[1] = {{.addr = &header, .size = sizeof header}};
    
  header.cmd = MSG_CMD_SETGPIO;
  header.u.setgpio.gpio = gpio;
  header.u.setgpio.state = state;
  
	return sendmsg(_gpio_fd, MSG_SUBCLASS_GPIO, 1, siov, 0, NULL);
}


/*
 *
 */
int get_gpio(int gpio)
{
  struct msg_gpio_req header;
  msgiov_t siov[1] = {{.addr = &header, .size = sizeof header}};
    
  header.cmd = MSG_CMD_GETGPIO;
  header.u.getgpio.gpio = gpio;
  
	return sendmsg(_gpio_fd, MSG_SUBCLASS_GPIO, 1, siov, 0, NULL);
}


