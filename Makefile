CC = gcc
CFLAGS = -O2 -Wall

all: src/libsauce.o t/saucetest.o
	$(CC) $(CFLAGS) t/saucetest.o src/libsauce.o -o t/saucetest

t/saucetest.o: t/test.c src/libsauce.h
	$(CC) $(CFLAGS) -c t/test.c -o t/saucetest.o

src/libsauce.o: src/libsauce.c src/libsauce.h
	$(CC) $(CFLAGS) -c src/libsauce.c -o src/libsauce.o

clean:
	@-rm -f src/*.o t/*.o t/saucetest
