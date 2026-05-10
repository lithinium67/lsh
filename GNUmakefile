CC = gcc
#CFLAGS = -I./include -Wall -Werror -pedantic -std=c99 -s -O3 # release
CFLAGS = -I./include -Wall -Werror -pedantic -std=c99 -g -O0 # debug
LIBS = $(shell pkg-config --libs readline)
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
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BDIR):
	mkdir -p $(BDIR)

clean:
	rm -rf $(BDIR)/* $(BDIR) src/*.o
