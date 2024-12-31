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

#ifndef SYS_SYSINIT_H
#define SYS_SYSINIT_H

#include <stdint.h>

// Pathname of sysinit service
#define SYSINIT_PATH              "/serv/sysinit"
#define SYSINIT_EXE_PATH          "/system/servers/sysinit"


// Message subclass for sysinit service
#define MSG_SUBCLASS_SYSINIT      2001


// Commands
#define SYSINIT_SHUTDOWN          0x00000001


/*
 *
 */
struct sysinit_req
{
  int cmd;
  
  union {
    struct {
      int how;
    } shutdown;
  } u;
};


/*
 * Prototypes
 */
int sysinit_shutdown(int how);

								

#endif
