CC = gcc
CPP = g++
CFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += -Wall -Wextra

EXECS = sh map
SRCS = memory/map.c memory/memory.c shell/sh.c shell/shell-lib.c util/util.c util/date.c util/strings.c util/convert.c \
freescaleK70/io.c freescaleK70/io_fs.c freescaleK70/io_button.c freescaleK70/io_led.c freescaleK70/button_flash.c

VPATH = init memory shell freescaleK70


.PHONY : all
all : $(EXECS)

.PHONY : clean
clean : clean-tests
	rm -f $(EXECS) *.o

# autmatically pull in dependencies on included header files
# copied from http://stackoverflow.com/a/2394668/1424966
depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >>./.depend
include .depend

include freescaleK70/Makefile

include memory/Makefile

include shell/Makefile

include init/Makefile

include test/Makefile
