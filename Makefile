CC = gcc

CFLAGS = -g -Wall

TARGET = main.out

all: $(TARGET)

SRCS = instruction.c main.c pipeline.c util.c

OBJS = $(SRCS:.c=.o)
 # OBJS = hello.o func.o

all: $(TARGET)
	@echo done.

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)

depend: $(SRCS)
	makedepend $^