#include "file_descriptor.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct
{
  int position;
  char *symbol;
} RedirectSymbol;

static RedirectSymbol get_symbol(char **arguments)
{
  RedirectSymbol redirect_symbol = {0};
  for (int i = 0; arguments[i] != NULL; i++)
  {
    if (strcmp(arguments[i], "1>") == 0 || arguments[i][0] == '>')
    {
      redirect_symbol.position = i;
      redirect_symbol.symbol = "1>";
      break;
    }
    else if (strcmp(arguments[i], "2>") == 0)
    {
      redirect_symbol.position = i;
      redirect_symbol.symbol = "2>";
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
    file_descriptor.new_fd = creat(arguments[redirect_symbol.position + 1], 0644);
    dup2(file_descriptor.new_fd, strcmp(redirect_symbol.symbol, "1>") == 0 ? STDOUT_FILENO : STDERR_FILENO);
    close(file_descriptor.new_fd);
  }
  return file_descriptor;
}