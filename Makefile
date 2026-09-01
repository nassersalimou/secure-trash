CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11

.PHONY: all clean

all: transpose_file transpose_mdp

transpose_file: transpose_file.c
	$(CC) $(CFLAGS) -o $@ $<

transpose_mdp: transpose_mdp.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) transpose_file transpose_mdp
