#ifndef MAIN_H
#define MAIN_H

#define LIST_OF_COMMANDS \
  GEN(ECHO)              \
  GEN(EXIT)              \
  GEN(TYPE)

#define GEN(name) CMD_##name,

enum commands
{
  LIST_OF_COMMANDS
};

#undef GEN
#endif // MAIN_H
