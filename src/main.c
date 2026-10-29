#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void parse_input(char *input)
{
  switch (buildin_command(input))
  {
  case cmd_echo:
    printf("%s\n", input + 5); // don't like that 5 is hard coded
  case cmd_exit:
    break;
  case cmd_type:
    if (buildin_command(input) != cmd_count)
    {
      printf("%s is a shell builtin\n", input + 5);
      break;
    }
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
