#include "os_utils.h"

#include <Windows.h>
#include <Shlwapi.h>
#include <UserEnv.h>

void os_windowsify_path(str8* unix_path)
{
  for (i32 c = 0; c < unix_path->length; ++c)
  {
    if (unix_path->buffer[c] == '/')
      unix_path->buffer[c] = '\\';
  }
}

void os_unixify_path(str8* windows_path)
{
  for (i32 c = 0; c < windows_path->length; ++c)
  {
    if (windows_path->buffer[c] == '\\')
      windows_path->buffer[c] = '/';
  }
}

void os_get_exe_path(str8* path)
{
  i32 l = GetModuleFileNameA(NULL, path->buffer, path->length);
  if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    path->length = -1;
  else
  {
    for (;path->buffer[l] != '\\'; --l);
    path->buffer[l] = '\0';
    path->length = l;
  }
}

void os_rel_path(str8 a, str8 b, str8* result)
{
  char* res = PathCombineA(result->buffer, a.buffer, b.buffer);
  if (res == NULL)
    result->length = -1;
  // TODO(calco): Figure out size of result.
}

u8 os_file_exists(str8 path)
{
  u32 file_attrib = GetFileAttributesA(path.buffer);

  //  TODO(calco): Maybe this doesnt cover quite all cases.
  if (file_attrib == INVALID_FILE_ATTRIBUTES &&
      GetLastError() == ERROR_FILE_NOT_FOUND)
  {
    return 0;
  }

  if (file_attrib & FILE_ATTRIBUTE_DIRECTORY)
    return 2;

  return 1;
}

void os_read_file(str8 path, str8* content)
{
  HANDLE file = CreateFileA(
      path.buffer, // path to file
      GENERIC_READ,  // access rights
      0,    // share stuff with os. 0 means nothing can read write and delete
      NULL, // some security things and stuff aaa
      OPEN_EXISTING,         // behaviour for stuff
      FILE_ATTRIBUTE_NORMAL, // Default file stuff
      NULL
  );

  // Get file size in bytes
  i32 size    = GetFileSize(file, NULL);
  if (size >= content->length)
  {
    content->length = -1;
    return;
  }

  DWORD bytes_read = 0;
  b32 success    = ReadFile(file, content->buffer, size, &bytes_read, NULL);
  content->length = success ? size : -1;

  CloseHandle(file);
}