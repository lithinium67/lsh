CC = tcc
CFLAGS = -I./include -Wall -Werror -pedantic $(CCFLAGS)
#CCFLAGS = -Os -s -fno-asynchronous-unwind-tables -std=c99 # on GCC
CCFLAGS = -s # on tcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BDIR = build
BNAME = shell

.PHONY: all clean run compile

all: run

run: compile
	./$(BDIR)/$(BNAME)

compile: $(BDIR) $(BDIR)/$(BNAME)

$(BDIR)/$(BNAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR):
	mkdir -p $(BDIR)

clean:
	rm -rf $(BDIR)/* $(BDIR) src/*.o
