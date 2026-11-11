#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>

static void count_words(char *input, int *count)
{
  int i;
  for (i = 0, *count = 0; input[i] != '\0'; i++)
  {
    if (input[i] != ' ' && (i == 0 || input[i - 1] == ' '))
      (*count)++;
  }
};

char **tokenize(char *input)
{
  char **string_array;
  char *save_ptr;
  char *token;
  int word_count;
  count_words(input, &word_count);

  string_array = malloc(sizeof(char) * strlen(input + 1));

  for (token = strtok_r(input, " ", &save_ptr); token != NULL; token = strtok_r(NULL, " ", &save_ptr))
  {
  }
};