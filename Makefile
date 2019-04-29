CC = gcc
CFLAGS = -g -O2
TARGET = my_shell
OBJECTS = execute.o parser.o redirect.o symbol.o command.o util.o
MAIN_SOURSE = my_shell.c

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(MAIN_SOURSE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
