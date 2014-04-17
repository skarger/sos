#ifndef IOFS_H
#define IOFS_H

#define MAX_OPEN_FILES 1024

Stream *fopen_fs(void);
int fclose_fs(Stream *stream);
int filename_valid(const char *filename);
int prefix_valid(const char *filename);
int basename_valid(char *basename);
int valid_basename_character(char c);
unsigned short next_file_id(void);
void initialize_io_fs(void);

#endif
