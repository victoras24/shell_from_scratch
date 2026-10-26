struct commands
{
  char *buffer;
  int count;
};

#define LIST_OF_COMMANDS \
  GEN(echo)              \
  GEN(exit)              \
  GEN(type)

#define GEN(name) char *name;

LIST_OF_COMMANDS

#undef GEN