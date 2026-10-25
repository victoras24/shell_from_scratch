#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL);

  while (1)
  {
    char input[1024];
    printf("$ ");
    char *input_position = fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if (input_position == NULL)
      return 0;
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
