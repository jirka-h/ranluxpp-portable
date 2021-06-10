OBJS	= ranluxpp.o ranluxpp-test.o
SOURCE	= ranluxpp.c ranluxpp-test.c
HEADER	= helpers.h mulmod.h ranluxpp.h
OUT	= ranluxpp-test
CC	 = gcc
CFLAGS   = -g -Wall -Wextra -Wpedantic -O3 -fsanitize=undefined

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(CFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJS) $(OUT)

arbitrary_precison_computation: arbitrary_precison_computation.c
	$(CC) $(CFLAGS) -o $@ $< -lgmp
