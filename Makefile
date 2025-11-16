CC = gcc
CFLAGS = -Wall -g
OBJ = hach.o memo.o parser.o cpu.o regexadr.o segcode.o main.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

hach.o: hach.c hach.h
	$(CC) $(CFLAGS) -c hach.c

memo.o: memo.c memo.h
	$(CC) $(CFLAGS) -c memo.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

cpu.o: cpu.c cpu.h
	$(CC) $(CFLAGS) -c cpu.c
	
regexadr.o: regexadr.c regexadr.h
	$(CC) $(CFLAGS) -c regexadr.c

segcode.o: segcode.c segcode.h
	$(CC) $(CFLAGS) -c segcode.c
		
main.o: main.c hach.h memo.h parser.h cpu.h regexadr.h segcode.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main
