CC=gcc
CFLAGS=-I.
DEPS = utility.h parse_commands.h pipe_commands.h history.h linked_list.h commands.h builtin.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ush: main.o utility.o parse_commands.o pipe_commands.o history.o linked_list.o commands.o builtin.o
	gcc -std=ansi -o $@ $^ $(CFLAGS)
