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
  strtok(input, " ");
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
  int cx;
  char directories[1024];
  char *path = getenv("PATH");
  char *path_cp = strdup(path);
  char *directory = strtok(path_cp, ":");
  printf("%s first run\n", directory);
  while (directory != NULL)
  {
    cx = snprintf(directories, sizeof(directories), "%s/%s", directory, command);
    if (access(cx, X_OK) == 0)
    {
      free(path_cp);
      return directory;
    };
    directory = strtok(NULL, ":");
    printf("%s rest runs\n", directory);
  }
  free(path_cp);
  return 0;
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
      printf("%s is a shell builtin\n", input_cp);
      break;
    }
    get_executable_path(input_cp);
    printf("%s: not found\n", input + 5);
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
