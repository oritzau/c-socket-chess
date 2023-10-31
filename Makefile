PROGRAM=chess
SRCS:= game.c main.c
	
CC=gcc
FLAGS:=-Wall -lm

all:$(PROGRAM)

network: server.c client.c 
	$(CC) -o server server.c $(FLAGS)
	$(CC) -o client client.c $(FLAGS)

test: test.c game.c
	$(CC) -o test test.c game.c $(FLAGS)

$(PROGRAM): $(SRCS)
	$(CC) $(SRCS) -o $(PROGRAM) $(FLAGS)

clean:; rm -f $(PROGRAM)
