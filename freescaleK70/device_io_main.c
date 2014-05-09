#ifdef IO_DEMO

#include "../include/constants.h"
#include "../init/init.h"
#include "../util/util.h"
#include "io_constants.h"
#include "io.h"
#include "hardware/delay.h"
#include "../memory/memory.h"
#include "hardware/svc.h"


void fputc_led_demo(char *led) {
    Stream *stream;
    stream = svc_myFopen(led);

    efputs("Turn on ", stdout);
    efputs(led, stdout);
    efputs("\n", stdout)
    ;
    svc_myFputc(1, stream);
    delay(1000000);

    efputs("Turn off ", stdout);
    efputs(led, stdout);
    efputs("\n", stdout);

    svc_myFputc(0, stream);
    svc_myFclose(stream);
}

void fgetc_button_demo(void) {
    efputs("A total of 6 button presses will terminate program\n", stdout);
    Stream *stream1 = svc_myFopen("/dev/button/sw1");
    Stream *stream2 = svc_myFopen("/dev/button/sw2");
    int counter = 0;
    int sw1, sw2;
    while(1) {
        sw1 = svc_myFgetc(stream1);
        if (sw1) {
            counter++;
            efputs("SW1 pressed\n", stdout);
        }
        sw2 = svc_myFgetc(stream2);
        if (sw2) {
            counter++;
            efputs("SW2 pressed\n", stdout);
        }
        if (counter >= 6) {
            svc_myFclose(stream1);
            svc_myFclose(stream2);
            break;
        }
    }
}

void fs_demo(void) {
    Stream *stream;
    int a, b, c;

    efputs("Create /dev/fs/text...\n", stdout);
    svc_myCreate("/dev/fs/text");

    efputs("Open a stream on /dev/fs/text...\n", stdout);
    stream = svc_myFopen("/dev/fs/text");

    efputs("Write characters a, b, c to the stream...\n", stdout);
    svc_myFputc('a', stream);
    svc_myFputc('b', stream);
    svc_myFputc('c', stream);

    efputs("Read a character and print it\n", stdout);
    a = svc_myFgetc(stream);
    efputc(a, stdout);
    efputc('\n', stdout);

    efputs("Read a character and print it\n", stdout);
    b = svc_myFgetc(stream);
    efputc(b, stdout);
    efputc('\n', stdout);

    efputs("Read a character and print it\n", stdout);
    c = svc_myFgetc(stream);
    efputc(c, stdout);
    efputc('\n', stdout);

    efputs("Close the stream\n", stdout);
    svc_myFclose(stream);

    efputs("Delete /dev/fs/text\n", stdout);
    svc_myDelete("/dev/fs/text");
}

int main(void) {
    initialize_system();
    initialize_memory();
    initialize_io();

    efputs("In memory file-system:\n", stdout);
    fs_demo();

    efputs("LEDs:\n", stdout);
    fputc_led_demo("/dev/led/orange");
    fputc_led_demo("/dev/led/yellow");
    fputc_led_demo("/dev/led/green");
    fputc_led_demo("/dev/led/blue");

    efputs("Buttons:\n", stdout);
    fgetc_button_demo();

    efputs("Goodbye\n", stdout);
    return 0;
}

#endif
