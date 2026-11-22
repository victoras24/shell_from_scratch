#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

typedef struct
{
  int new_fd;
  int stdout_backup_fd;
} FileDescriptor;

FileDescriptor redirect_command(char **arguments);

#endif