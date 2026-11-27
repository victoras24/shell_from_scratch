#include "main.h"
#include "dispatch.h"
#include "main.h"
#include "tokenizer.h"
#include "line_editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  setbuf(stdout, NULL);
  save_original_settings();

  while (1)
  {
    int is_exit;
    char input[1024];
    int size = sizeof(input);
    int count = 0;
    int line_editor_res;

    printf("$ ");
    enable_raw_mode();
    line_editor_res = line_editor(input, &count, size);
    disable_raw_mode();
    if (line_editor_res == -1)
      break;

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
    {
      break;
    }
    else
      continue;
  }
}
