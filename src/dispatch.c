#include "dispatch.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int builtin_type(char **arguments);
static int builtin_echo(char **arguments);
static int builtin_exit(char **arguments);

struct builtin_command
{
  const char *name;
  int (*function)(char **arguments);
};

static struct builtin_command builtin_commands[] = {
    {"type", builtin_type},
    {"echo", builtin_echo},
    {"exit", builtin_exit},
};

static char *is_builtin_command(char *command)
{
  for (size_t i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]);
       i++)
  {
    if (strcmp(command, builtin_commands[i].name) == 0)
      return command;
  }
  return NULL;
}

static char *get_executable_path(char *command)
{
  const char *seperator = ":";
  const char *path = getenv("PATH");
  char *path_cp = strdup(path);
  char *save_ptr = NULL;
  char *next_directory = strtok_r(path_cp, seperator, &save_ptr);
  char executable_directory[PATH_MAX];

  while (next_directory)
  {
    snprintf(executable_directory, sizeof(executable_directory), "%s/%s",
             next_directory, command);
    if (access(executable_directory, X_OK) == 0)
    {
      free(path_cp);
      return strdup(executable_directory);
    }
    next_directory = strtok_r(NULL, seperator, &save_ptr);
  };

  free(path_cp);
  return NULL;
};

int builtin_type(char **arguments)
{
  for (int i = 1; arguments[i] != NULL; i++)
  {
    char *command = arguments[i];
    if (is_builtin_command(command))
    {
      printf("%s is a shell builtin\n", command);
      return 1;
    }
    else
    {
      char *executable_path = get_executable_path(command);
      if (executable_path != NULL)
        printf("%s is %s\n", command, executable_path);
      else
      {
        printf("%s: not found\n", command);
      }
      free(executable_path);
    }
  };
  return 1;
}

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

static int builtin_exit(char **arguments) { return 0; }

int dispatch(char **arguments)
{
  int is_continue = 1;
  int is_builtin_found = 0;
  char *command = arguments[0];

  for (size_t i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]);
       i++)
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
