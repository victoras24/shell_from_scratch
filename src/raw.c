#include <termios.h>
#include <unistd.h>
#include <stdio.h>

enum action_key
{
  ENTER = '\n',
  BACKSPACE = '\b'
};

void read_line(char *input, char *buff, int *count)
{
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1)
  {
    switch (c)
    {
    case ENTER:
      buff[*count] = '\0';
      (*count)++;
      break;
    case BACKSPACE:
      buff[*count - 1] = '\0';
      (*count)--;
      break;
    default:
      buff[*count] = c;
      (*count)++;
      break;
    }
  }
}

int main()
{
  struct termios original_settings;
  struct termios raw_settings;

  if (tcgetattr(STDIN_FILENO, &original_settings) == -1)
  {
    perror("tcgetattr");
    return 1;
  }

  raw_settings = original_settings;
  raw_settings.c_lflag &= ~(ICANON | ECHO);
  if (tcsetattr(STDIN_FILENO, TCSANOW, &raw_settings) == -1)
  {
    perror("tcsetattr");
    return 1;
  }

  char c;

  while (read(STDIN_FILENO, &c, 1) == 1)
  {
    if (c == 'q')
    {
      break;
    }
    printf("You pressed: '%c' (ASCII: %d)\n", c, c);
  }

  if (tcsetattr(STDIN_FILENO, TCSANOW, &original_settings) == -1)
  {
    perror("tcsetattr restore");
    return 1;
  }

  return 0;
};