CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g

all: bruteforce divideAndConquer

clean:
	rm -f *.o bruteforce divideAndConquer

bruteforce: bruteforceConvexHull.c 
	$(CC) $(CFLAGS) bruteforceConvexHull.c -lm -o bruteforce

divideAndConquer: divideConvexHull.c
	$(CC) $(CFLAGS) divideConvexHull.c -lm -o divideAndConquer