#include "file_descriptor.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

typedef struct
{
  char *symbol;
  int file_descriptor;
  int flags;
  int position;
} RedirectCommand;

static RedirectCommand redirects[] = {
    {">", STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC, 0},
    {"1>", STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC, 0},
    {"2>", STDERR_FILENO, O_WRONLY | O_CREAT | O_TRUNC, 0},
    {">>", STDOUT_FILENO, O_WRONLY | O_CREAT | O_APPEND, 0},
    {"1>>", STDOUT_FILENO, O_WRONLY | O_CREAT | O_APPEND, 0},
    {"2>>", STDERR_FILENO, O_WRONLY | O_CREAT | O_APPEND, 0},
};

static RedirectCommand *get_redirect(char **arguments)
{
  for (int i = 0; arguments[i] != NULL; i++)
  {
    for (size_t j = 0; j < sizeof(redirects) / sizeof(redirects[0]); j++)
    {
      if (strcmp(arguments[i], redirects[j].symbol) == 0)
      {
        arguments[i] = NULL;
        redirects[j].position = i;
        return &redirects[j];
      }
    }
  }
  return NULL;
}

FileDescriptor redirect_file_descriptor(char **arguments)
{
  FileDescriptor file_descriptor;
  file_descriptor.stdout_backup_fd = dup(STDOUT_FILENO);
  RedirectCommand *redirect = get_redirect(arguments);
  if (redirect != NULL)
  {
    file_descriptor.new_fd = open(arguments[redirect->position + 1], redirect->flags, 0644);
    dup2(file_descriptor.new_fd, redirect->file_descriptor);
    close(file_descriptor.new_fd);
  }
  return file_descriptor;
}