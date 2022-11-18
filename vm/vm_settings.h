#pragma once

#ifndef VM_SETTINGS_H
#define VM_SETTINGS_H

#define PAGE_MEMORY_SIZE 4096
#define STACK_MEMORY_SIZE PAGE_MEMORY_SIZE * 2
#define HEAP_MEMORY_SIZE PAGE_MEMORY_SIZE * 12
#define ADDRESS_LEN 2
#define DIFERENT_VAR_NAMES 256
#define ADDRESSES_TABLE_SIZE DIFERENT_VAR_NAMES * ADDRESS_LEN

/*  Returns true if given memory address is greater than max size or less than 0 */
#define CHECK_STACK_MEMORY_LIMITS(_) (_ > STACK_MEMORY_SIZE || _ <= 0)

#endif