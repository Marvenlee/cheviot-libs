/*
 * Copyright 2014  Marven Gilhespie
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


// @brief   VM and page table definitions 
.equ PAGE_SIZE,                       4096

// @brief   Kernel - User VM Boundaries
.equ VM_BOOTLOADER_CEILING,           0x00010000
.equ VM_USER_BASE_PAGETABLE_ALIGNED,  0x00000000
.equ VM_USER_BASE,                    0x00008000
.equ VM_USER_CEILING,                 0x7F000000

.equ VM_KERNEL_BASE,                  0x80000000
.equ VM_IO_BASE,                      0xFFF00000
.equ VM_KERNEL_CEILING,               0xFFFFF000

