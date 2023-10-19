PROGRAM=chess
SRCS:= board.c
	
CC=gcc
FLAGS:=-Wall -lm

all:$(PROGRAM)

network: server.c client.c 
	$(CC) -o server server.c $(FLAGS)
	$(CC) -o client client.c $(FLAGS)

$(PROGRAM): $(SRCS)
	$(CC) $(SRCS) -o $(PROGRAM) $(FLAGS)

clean:; rm -f $(PROGRAM)
