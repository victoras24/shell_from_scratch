#include "main.h"
#include "dispatch.h"
#include "main.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEN(name) #name,
const char *build_in_commands[] = {LIST_OF_COMMANDS};
#undef GEN

int main() {
  setbuf(stdout, NULL);

  while (1) {
    int is_continue;
    char input[1024];

    printf("$ ");
    if (fgets(input, sizeof(input), stdin) == NULL)
      return 0;
    input[strcspn(input, "\n")] = '\0';

    char **arguments = tokenize(input);
    if (arguments == NULL || arguments[0] == NULL) {
      free(arguments);
      continue;
    }

    is_continue = dispatch(arguments);
    free(arguments);
    if (is_continue == 1)
      continue;
    else
      break;
  }
}
