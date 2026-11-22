#include "file_descriptor.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

static int get_redirect_command_pos(char **arguments)
{
  int redirect_command_pos = 0;
  for (int i = 0; arguments[i] != NULL; i++)
  {
    if (strcmp(arguments[i], "1>") == 0 || arguments[i][0] == '>')
    {
      redirect_command_pos = i;
      break;
    }
  }
  return redirect_command_pos;
}

FileDescriptor redirect_command(char **arguments)
{
  FileDescriptor file_descriptor;
  file_descriptor.stdout_backup_fd = dup(STDOUT_FILENO);
  int redirect_command_pos = get_redirect_command_pos(arguments);
  if (redirect_command_pos != 0)
  {
    arguments[redirect_command_pos] = NULL;
    file_descriptor.new_fd = creat(arguments[redirect_command_pos + 1], 0644);
    dup2(file_descriptor.new_fd, STDOUT_FILENO);
    close(file_descriptor.new_fd);
  }
  return file_descriptor;
}