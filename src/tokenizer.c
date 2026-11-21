#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum quote_state
{
  UNQUOTED_STATE,
  SINGLE_QUOTED_STATE,
  DOUBLE_QUOTED_STATE,
};

enum symbol
{
  SPACE = ' ',
  SINGLE_QUOTES = '\'',
  DOUBLE_QUOTES = '"',
  BACKSLASHES = '\\',
};

static void push_token(char *token, int *token_index, char **tokens, int *tokens_index, int *need_new_token)
{
  token[*token_index] = '\0';
  tokens[*tokens_index] = token;
  (*tokens_index)++;
  *token_index = 0;
  *need_new_token = 1;
}

static void push_char(char *token, char character, int *token_index)
{
  token[*token_index] = character;
  (*token_index)++;
}

static void count_words(const char *input, int *count)
{
  int i;
  for (i = 0, *count = 0; input[i] != '\0'; i++)
  {
    if (input[i] != ' ' && (i == 0 || input[i - 1] == ' '))
      (*count)++;
  }
}

char **tokenize(const char *input)
{
  enum quote_state quote_state = UNQUOTED_STATE;
  char **tokens;
  int tokens_index;
  char *token;
  int index;
  int word_count;
  int token_index;
  int need_new_token = 1;

  count_words(input, &word_count);
  tokens = malloc(sizeof(*tokens) * (word_count + 1));
  if (tokens == NULL)
    return NULL;

  for (index = 0, tokens_index = 0, token_index = 0; input[index] != '\0'; index++)
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
          push_token(token, &token_index, tokens, &tokens_index, &need_new_token);
        }
        break;
      case SINGLE_QUOTES:
        quote_state = SINGLE_QUOTED_STATE;
        break;
      case DOUBLE_QUOTES:
        quote_state = DOUBLE_QUOTED_STATE;
        break;
      case BACKSLASHES:
        if (index[input + 1] != ' ')
        {
          push_char(token, input[index + 1], &token_index);
          index++;
        }
        break;
      default:
        push_char(token, input[index], &token_index);
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
        push_char(token, input[index], &token_index);
        break;
      }
      break;
    case DOUBLE_QUOTED_STATE:
      switch (input[index])
      {
      case DOUBLE_QUOTES:
        quote_state = UNQUOTED_STATE;
        break;
      case BACKSLASHES:
        push_char(token, input[index + 1], &token_index);
        index++;
        break;
      default:
        push_char(token, input[index], &token_index);
        break;
      }
      break;
    default:
      break;
    }
  }

  if (need_new_token == 0)
  {
    push_token(token, &token_index, tokens, &tokens_index, &need_new_token);
  }
  tokens[tokens_index] = NULL;
  return tokens;
}