
CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/main.c src/process.c src/scheduler.c src/utils.c policies/fifo.c policies/sjf.c policies/rr.c
OBJ = $(SRC:.c=.o)

EXEC = ordonnanceur

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)
