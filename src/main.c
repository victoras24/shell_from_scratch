#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  setbuf(stdout, NULL);
  char input[1024];
  printf("$ ");
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';
  printf("%s: command not found\n", input);
}
