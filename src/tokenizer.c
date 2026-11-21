#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum quote_state
{
  UNQUOTED_STATE,
  SINGLE_QUOTED_STATE,
  DOUBLE_QUOTED_STATE,
  BACKSLASHED_STATE
};

enum symbol
{
  SPACE = ' ',
  SINGLE_QUOTES = '\'',
  DOUBLE_QUOTES = '"',
  BACKSLASHES = '\\',
};

static void push_token_to_string_array(char *token, int *token_index, char **string_array, int *string_array_index, int *need_new_token)
{
  token[*token_index] = '\0';
  string_array[*string_array_index] = token;
  (*string_array_index)++;
  *token_index = 0;
  *need_new_token = 1;
}

static void push_char_to_token(char *token, char character, int *token_index)
{
  token[*token_index] = character;
  (*token_index)++;
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
  enum quote_state quote_state = UNQUOTED_STATE;
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

    if (need_new_token == 1 && input[index] != ' ')
    {
      token = malloc(strlen(input) + 1);
      if (token == NULL)
        return NULL;
      need_new_token = 0;
    }

    switch (quote_state)
    {
    case UNQUOTED_STATE:
      switch (input[index])
      {
      case SPACE:
        if (need_new_token == 0)
        {
          push_token_to_string_array(token, &token_index, string_array, &string_array_index, &need_new_token);
        }
        break;
      case SINGLE_QUOTES:
        quote_state = SINGLE_QUOTED_STATE;
        break;
      case DOUBLE_QUOTES:
        quote_state = DOUBLE_QUOTED_STATE;
        break;
      case BACKSLASHES:
        quote_state = BACKSLASHED_STATE;
        break;
      default:
        push_char_to_token(token, input[index], &token_index);
        break;
      }
      break;
    case SINGLE_QUOTED_STATE:
      switch (input[index])
      {
      case SINGLE_QUOTES:
        quote_state = UNQUOTED_STATE;
        break;
      default:
        push_char_to_token(token, input[index], &token_index);
        break;
      }
      break;
    case DOUBLE_QUOTED_STATE:
      switch (input[index])
      {
      case DOUBLE_QUOTES:
        quote_state = UNQUOTED_STATE;
        break;
      default:
        push_char_to_token(token, input[index], &token_index);
        break;
      }
      break;
    case BACKSLASHED_STATE:
      push_char_to_token(token, input[index], &token_index);
      quote_state = UNQUOTED_STATE;
    default:
      break;
    }
  }

  if (need_new_token == 0)
  {
    push_token_to_string_array(token, &token_index, string_array, &string_array_index, &need_new_token);
  }
  string_array[string_array_index] = NULL;
  return string_array;
}