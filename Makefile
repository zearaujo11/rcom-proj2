# Makefile to build the project
# NOTE: This file must not be changed.

# Parameters
CC = gcc
CFLAGS = -Wall -lm

SRC = src/
INCLUDE = include/

# Targets
build: download 

download: main.c $(SRC)/*.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDE)

clean:
	rm -f download
