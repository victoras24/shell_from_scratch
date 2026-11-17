#include "dispatch.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void builtin_type(char **arguments);
static void builtin_echo(char **arguments);
static void builtin_exit(char **arguments);
static void builtin_pwd(char **arguments);
static void builtin_cd(char **arguments);

struct builtin_command
{
  const char *name;
  void (*function)(char **arguments);
  int is_exit;
};

static struct builtin_command builtin_commands[] = {
    {"type", builtin_type, 0},
    {"echo", builtin_echo, 0},
    {"exit", builtin_exit, 1},
    {"pwd", builtin_pwd, 0},
    {"cd", builtin_cd, 0},
};

static const struct builtin_command *get_builtin_command(char *command)
{
  for (size_t i = 0; i < sizeof(builtin_commands) / sizeof(builtin_commands[0]);
       i++)
  {
    if (strcmp(command, builtin_commands[i].name) == 0)
      return &builtin_commands[i];
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

static void builtin_type(char **arguments)
{
  for (int i = 1; arguments[i] != NULL; i++)
  {
    char *command = arguments[i];
    if (get_builtin_command(command))
    {
      printf("%s is a shell builtin\n", command);
    }
    else
    {
      char *executable_path = get_executable_path(command);
      if (executable_path != NULL)
        printf("%s is %s\n", command, executable_path);
      else
      {
        printf("%s: not found\n", command);
        break;
      }
      free(executable_path);
    }
  };
}

static void builtin_echo(char **arguments)
{
  for (int i = 1; arguments[i] != NULL; i++)
  {
    if (arguments[i + 1] != NULL)
      printf("%s ", arguments[i]);
    else
      printf("%s", arguments[i]);
  }
  printf("%s", "\n");
}

static void builtin_exit(char **arguments) {}

static void builtin_pwd(char **arguments)
{
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
    printf("%s\n", cwd);
  else
    perror("getcwd() error");
}

static void builtin_cd(char **arguments)
{
  char *home_dir = getenv("HOME");
  char *last_dir = getenv("OLDPWD");
  char *path = arguments[1];

  if (path == NULL)
    chdir(home_dir);
  else
  {
    if (strcmp(path, "-") == 0)
      chdir(last_dir);
    else
      chdir(path);
  }
}

static void execute_path(char **arguments)
{
  pid_t pid = 0;
  int wait_pid_log_number;
  char *executable_path;
  char *command = arguments[0];

  executable_path = get_executable_path(command);

  if (executable_path != NULL)
  {
    pid = fork();

    switch (pid)
    {
    case -1:
    {
      perror("fork");
      break;
    }
    case 0:
    {
      if (execv(executable_path, arguments) == -1)
      {
        perror(command);
        exit(EXIT_FAILURE);
      }
    }
    default:
    {
      pid_t waited_pid = waitpid(pid, &wait_pid_log_number, 0);
      if (waited_pid == -1)
        perror("waitpid failed");
      break;
    }
    }
  }
  else
  {
    printf("%s: command not found\n", command);
  }
  free(executable_path);
}

int dispatch(char **arguments)
{
  char *command = arguments[0];
  const struct builtin_command *builtin_command = get_builtin_command(command);

  if (builtin_command != NULL)
  {
    builtin_command->function(arguments);
    return builtin_command->is_exit;
  }
  else
  {
    execute_path(arguments);
    return 0;
  }
};
