#ifndef DISPATCH_H
#define DISPATCH_H

int dispatch(char **arguments);
const char *get_autocompleted_command_name(char *input, int *input_len);

#endif // DISPATCH_H