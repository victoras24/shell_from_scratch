#include "file_descriptor.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct
{
  int position;
  char *symbol;
  int file_descriptor;
} RedirectSymbol;

static RedirectSymbol get_symbol(char **arguments)
{
  RedirectSymbol redirect_symbol = {0};
  for (int i = 0; arguments[i] != NULL; i++)
  {
    if (strcmp(arguments[i], "1>") == 0 || strcmp(arguments[i], ">") == 0)
    {
      redirect_symbol.position = i;
      redirect_symbol.symbol = "1>";
      redirect_symbol.file_descriptor = STDOUT_FILENO;
      break;
    }
    else if (strcmp(arguments[i], "2>") == 0)
    {
      redirect_symbol.position = i;
      redirect_symbol.symbol = "2>";
      redirect_symbol.file_descriptor = STDERR_FILENO;
      break;
    }
    else if (strcmp(arguments[i], "1>>") == 0 || (strcmp(arguments[i], ">>") == 0))
    {
      redirect_symbol.position = i;
      redirect_symbol.symbol = ">>";
      redirect_symbol.file_descriptor = STDOUT_FILENO;
      break;
    }
  }
  return redirect_symbol;
}

FileDescriptor redirect_file_descriptor(char **arguments)
{
  FileDescriptor file_descriptor;
  file_descriptor.stdout_backup_fd = dup(STDOUT_FILENO);
  RedirectSymbol redirect_symbol = get_symbol(arguments);
  if (redirect_symbol.position != 0)
  {
    arguments[redirect_symbol.position] = NULL;
    if (strcmp(redirect_symbol.symbol, ">>") == 0)
      file_descriptor.new_fd = open(arguments[redirect_symbol.position + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
      file_descriptor.new_fd = creat(arguments[redirect_symbol.position + 1], 0644);
    dup2(file_descriptor.new_fd, redirect_symbol.file_descriptor);
    close(file_descriptor.new_fd);
  }
  return file_descriptor;
}