#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
  int string_array_index;
  char *token;
  int word_count;
  int index;
  int token_index;
  count_words(input, &word_count);
  int is_inside_single_quotes = 0;
  int need_new_token = 1;

  string_array = malloc(sizeof(*string_array) * (word_count + 1));
  if (string_array == NULL)
    return NULL;

  for (index = 0, string_array_index = 0, token_index = 0; input[index] != '\0'; index++)
  {
    if (need_new_token == 1 && input[index] != ' ')
    {
      token = malloc(strlen(input) + 1);
      if (token == NULL)
        return NULL;
      need_new_token = 0;
    }

    if (input[index] == ' ' && need_new_token == 0)
    {
      token[token_index] = '\0';
      token_index = 0;
      string_array[string_array_index] = token;
      string_array_index++;
      need_new_token = 1;
      continue;
    }

    if (input[index] != ' ')
    {
      token[token_index] = input[index];
      token_index++;
    }
  }

  if (need_new_token == 0)
  {
    token[token_index] = '\0';
    string_array[string_array_index] = token;
    string_array_index++;
  }
  string_array[string_array_index] = NULL;

  for (int i = 0; string_array[i] != NULL; i++)
  {
    printf("this is print: %s\n", string_array[i]);
  }

  return string_array;
}