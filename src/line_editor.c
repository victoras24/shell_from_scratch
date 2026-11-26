#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "line_editor.h"

enum action_key_ascii
{
  ENTER = 10,
  BACKSPACE = 127
};

static struct termios original_settings;
static struct termios raw_settings;

void enable_raw_mode()
{
  if (tcgetattr(STDIN_FILENO, &original_settings) == -1)
    perror("tcgetattr");

  raw_settings = original_settings;
  raw_settings.c_lflag &= ~(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &raw_settings) == -1)
    perror("tcsetattr");
};

void disable_raw_mode()
{
  if (tcsetattr(STDIN_FILENO, TCSANOW, &original_settings) == -1)
    perror("tcsetattr restore");
}

int line_editor(char *buff, int *count, int size)
{
  char c = '\0';

  while (read(STDIN_FILENO, &c, 1) == 1)
  {
    switch (c)
    {
    case ENTER:
      buff[*count] = '\0';
      printf("%s", "\n");
      return *count;
    case BACKSPACE:
      if (*count == 0)
        break;
      printf("%c", '\b');
      printf("%c", ' ');
      printf("%c", '\b');
      (*count)--;
      break;
    default:
      if (*count >= (size - 1))
        break;
      buff[*count] = c;
      printf("%c", c);
      (*count)++;
      break;
    }
  }

  return *count;
}