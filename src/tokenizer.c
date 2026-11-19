#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum quote_state
{
  UNQUOTED,
  SINGLE_QUOTED,
  DOUBLE_QUOTED
};

static void push_token(char *token, int token_index, char **string_array, int *string_array_index)
{
  token[token_index] = '\0';
  string_array[*string_array_index] = token;
  (*string_array_index)++;
}

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
  int quote_state = 0;
  char **string_array;
  int string_array_index;
  char *token;
  int index;
  int word_count;
  int token_index;
  int need_new_token = 1;

  count_words(input, &word_count);
  string_array = malloc(sizeof(*string_array) * (word_count + 1));
  if (string_array == NULL)
    return NULL;

  for (index = 0, string_array_index = 0, token_index = 0; input[index] != '\0'; index++)
  {

    if (need_new_token == 1)
    {
      token = malloc(strlen(input) + 1);
      if (token == NULL)
        return NULL;
      need_new_token = 0;
    }

    switch (quote_state)
    {
    case UNQUOTED:
      if (input[index] != ' ' && input[index] != '\'' && input[index] != '\"')
      {
        token[token_index] = input[index];
        token_index++;
      }
      else if (input[index] == '\'')
        quote_state = SINGLE_QUOTED;
      else if (input[index] == '\"')
        quote_state = DOUBLE_QUOTED;
      else if (input[index] == ' ' && input[index - 1] != ' ')
      {
        push_token(token, token_index, string_array, &string_array_index);
        token_index = 0;
        need_new_token = 1;
      }
      break;
    case SINGLE_QUOTED:
      if (input[index] == '\'')
      {
        quote_state = UNQUOTED;
        break;
      }
      token[token_index] = input[index];
      token_index++;
      break;
    case DOUBLE_QUOTED:
      if (input[index] == '\"')
      {
        quote_state = UNQUOTED;
        break;
      }
      token[token_index] = input[index];
      token_index++;
      break;
    default:
      break;
    }
  }

  if (need_new_token == 0)
  {
    push_token(token, token_index, string_array, &string_array_index);
  }
  string_array[string_array_index] = NULL;
  return string_array;
}