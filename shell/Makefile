SHELL_STANDALONE_DEPS = sh.o shell-lib-standalone.o memory.o util.o strings.o date.o convert.o \
shell-io.o io-standalone.o io_fs.o

SHELL_DEPS_WITH_DEVICES = sh.o shell-lib.o memory.o date.o util.o strings.o \
shell-io.o io.o io_led.o io_button.o io_fs.o

sh : $(SHELL_STANDALONE_DEPS)
	$(CC) $(CFLAGS) $(SHELL_STANDALONE_DEPS) -U K70 -o $@

sh.o : sh.c

shell-lib-standalone.o : shell-lib.c
	$(CC) $(CFLAGS) -c shell/shell-lib.c -U K70 -o $@

shell-lib.o : shell-lib.c
	$(CC) $(CFLAGS) -c shell/shell-lib.c -D K70 -o $@

shell-io.o : shell-io.c

ser2lcd.o : ser2lcd.c
touch2led.o : touch2led.c
pot2ser.o : pot2ser.c
therm2ser.c : therm2ser.c
pb2led.o : pb2led.c

date.o : util/date.c
	$(CC) $(CFLAGS) -c util/date.c -o $@

strings.o : util/strings.c
	$(CC) $(CFLAGS) -c util/strings.c -o $@

util.o : util/util.c
	$(CC) $(CFLAGS) -c util/util.c -D K70 -o $@

convert.o : util/convert.c
	$(CC) $(CFLAGS) -c util/convert.c -o $@