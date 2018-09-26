CC=gcc
DEPS = header.h
OBJ = builtin.o setup.o shell.o run.o pinfo.o parser.o io_redirect.o helper_funcs.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)
