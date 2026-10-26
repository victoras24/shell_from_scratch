#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

const char *build_in_commands[] = {"echo", "exit", "type"};

int is_buildin_command(char *input)
{
  for (size_t i = 0; i < (sizeof(build_in_commands) / sizeof(build_in_commands[0])); i++)
  {
    if (strcmp(build_in_commands[i], input) == 0)
      return 1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL);

  while (1)
  {
    char input[1024]; // this could be created at the preprossor
    printf("$ ");
    char *input_str = fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (input_str == NULL)
      return 0;
    if (strncmp(input, "type ", 5) == 0)
    {
      if (is_buildin_command(input + 5))
      {
        printf("%s is a shell builtin\n", input + 5);
        continue;
      }
      else
      {
        printf("%s: not found\n", input + 5);
        continue;
      }
    }

    if (strncmp(input, "echo ", 5) == 0)
    {
      printf("%s\n", input + 5);
      continue;
    }

    if (strcmp(input, "exit") == 0)
      return 0;

    printf("%s: command not found\n", input);
  }
}
