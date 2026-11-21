#include "main.h"
#include "dispatch.h"
#include "main.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  setbuf(stdout, NULL);

  while (1)
  {
    int is_exit;
    char input[1024];

    printf("$ ");
    if (fgets(input, sizeof(input), stdin) == NULL)
      return 0;
    input[strcspn(input, "\n")] = '\0';

    char **arguments = tokenize(input);

    if (arguments == NULL || arguments[0] == NULL)
    {
      free(arguments);
      continue;
    }

    is_exit = dispatch(arguments);

    for (size_t i = 0; arguments[i] != NULL; i++)
    {
      free(arguments[i]);
    };

    free(arguments);
    if (is_exit == 1)
      break;
    else
      continue;
  }
}
