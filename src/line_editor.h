#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

int line_editor(char *buff, int *count, int size);
void save_original_settings();
void enable_raw_mode();
void disable_raw_mode();

#endif // LINE_EDITOR_H
