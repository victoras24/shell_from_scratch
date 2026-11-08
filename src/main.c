#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

#define GEN(name) #name,
const char *build_in_commands[] = {LIST_OF_COMMANDS};
#undef GEN

char *parse_command(char *input)
{
  char *space = strchr(input, ' ');
  if (space == NULL)
    return 0;
  *space = '\0';
  return input;
};

char *get_input(char *input)
{
  char *input_cp = strdup(input);
  strtok(input_cp, " ");
  char *command = strtok(NULL, " ");
  if (command != NULL)
    return command;
  return 0;
};

int buildin_command(char *input)
{
  parse_command(input);
  for (size_t i = 0; i < cmd_count; i++)
  {
    if (input != NULL)
      if (strcmp(build_in_commands[i], input) == 0)
        return i;
  }
  return cmd_count;
}

char *get_executable_path(char *command)
{
  const char *seperator = ":";
  const char *path = getenv("PATH");
  char *path_cp = strdup(path);
  char *save_ptr = NULL;
  char *next_directory = strtok_r(path_cp, seperator, &save_ptr);
  char executable_directory[256];

  while (next_directory)
  {
    snprintf(executable_directory, sizeof(executable_directory), "%s/%s", next_directory, command);
    if (access(executable_directory, X_OK) == 0)
    {
      free(path_cp);
      printf("%s is %s\n", command, executable_directory);
      return strdup(executable_directory);
    }
    next_directory = strtok_r(NULL, seperator, &save_ptr);
  };

  free(path_cp);
  printf("%s: not found\n", command);
  return NULL;
};

void parse_input(char *input)
{
  char input_cp[1024];
  strcpy(input_cp, input);
  int command = buildin_command(input);
  int type_command = buildin_command(get_input(input_cp));
  switch (command)
  {
  case cmd_echo:
    printf("%s\n", input + 5); // don't like that 5 is hard coded
  case cmd_exit:
    break;
  case cmd_type:
    if (type_command != cmd_count)
    {
      printf("%s is a shell builtin\n", get_input(input_cp));
      break;
    }
    else
    {
      get_executable_path(get_input(input_cp));
    }
    break;
  default:
    printf("%s: command not found\n", input);
    break;
  }
}

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL);

  while (1)
  {
    char input[1024];
    printf("$ ");
    if (fgets(input, sizeof(input), stdin) == NULL)
      return 0;
    input[strcspn(input, "\n")] = '\0';
    parse_input(input);
    if (strcmp(input, "exit") == 0)
      return 0;
  }
}
