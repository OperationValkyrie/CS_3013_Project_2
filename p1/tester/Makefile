# Jonathan Chang																																																																																																																																																																																																																																																																																																																																																																																												# Jonathan Chang
# CS 3013 C02
# Project 2

CFLAGS = -Wall -g

all: tester

tester: tester.o
	gcc $(CFLAGS) tester.o -o tester

tester.o: tester.c tester.h
	gcc $(CFLAGS) -c tester.c

clean:
	rm -f *.o tester