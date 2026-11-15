#ifndef MAIN_H
#define MAIN_H

#define LIST_OF_COMMANDS \
  GEN(echo)              \
  GEN(exit)              \
  GEN(type)

#define GEN(name) cmd_##name,

enum commands
{
  LIST_OF_COMMANDS cmd_count
};

#undef GEN
#endif // MAIN_H
