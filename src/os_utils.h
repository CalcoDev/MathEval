#ifndef OS_UTILS_H
#define OS_UTILS_H

#include "defines.h"
#include "str8.h"

void os_windowsify_path(str8* unix_path);
void os_unixify_path(str8* windows_path);

void os_get_exe_path(str8* path);
void os_rel_path(str8 a, str8 b, str8* result);

u8 os_file_exists(str8 path);
void os_read_file(str8 path, str8* content);

#endif