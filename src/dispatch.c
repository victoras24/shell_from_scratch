#include "dispatch.h"
#include <stdio.h>
#include <string.h>

struct builtin_command
{
  const char *name;
  int (*function)(char **arguments);
};

static int builtin_echo(char **arguments)
{
  for (int i = 1; arguments[i] != NULL; i++)
  {
    if (arguments[i + 1] != NULL)
      printf("%s ", arguments[i]);
    else
      printf("%s", arguments[i]);
  }
  printf("%s", "\n");
  return 1;
}

static int builtin_exit(char **arguments)
{
  return 0;
}

static struct builtin_command builtin_commands[] = {
    {"echo", builtin_echo},
    {"exit", builtin_exit}};

int dispatch(char **arguments)
{
  int is_continue = 1;
  int is_builtin_found = 0;
  char *command = arguments[0];

  for (size_t i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]); i++)
  {
    if (strcmp(command, builtin_commands[i].name) == 0)
    {
      is_continue = builtin_commands[i].function(arguments);
      is_builtin_found = 1;
      break;
    }
  }

  if (is_builtin_found == 0)
    printf("%s: command not found\n", command);

  return is_continue;
};
