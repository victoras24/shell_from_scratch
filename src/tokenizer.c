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
}

char **tokenize(char *input)
{
  char **string_array;
  char *save_ptr;
  char *token;
  int word_count;
  int index;
  count_words(input, &word_count);

  string_array = malloc(sizeof(*string_array) * (word_count + 1));
  if (string_array == NULL)
    return NULL;

  for (index = 0, token = strtok_r(input, " ", &save_ptr); token != NULL; token = strtok_r(NULL, " ", &save_ptr), index++)
  {
    string_array[index] = token;
  }

  string_array[index] = NULL;
  return string_array;
}