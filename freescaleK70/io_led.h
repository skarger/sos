#ifndef IO_LED_H
#define IO_LED_H

#include "io.h"

#define NUMBER_LEDS 4

void initialize_io_led(void);

Stream *find_stream_led(enum device_instance di);

Stream *fopen_led(enum device_instance di);
int fclose_led(Stream *stream);

int fgetc_led(void);
int fputc_led(int c, Stream *stream);

void turn_on_led(enum device_instance di);
void turn_off_led(enum device_instance di);

#endif
