NAME = fifo_echo_server.exe

SRC = src/main.c

all:
	gcc $(SRC) -o $(NAME)
