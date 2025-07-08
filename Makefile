# Makefile for button_blinds executable

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpigpiod_if2 -lrt -lm


# Target executable name
TARGET = button_blinds


# Source files
SRC = blinds.c

# Default target
all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)


clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/


.PHONY: all clean install
