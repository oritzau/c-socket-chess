#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Usage: %s <port_number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int portnumber = atoi(argv[1]);
    if (portnumber < 9000) {
        printf("Invalid port number: %d > 9000", portnumber);
        return EXIT_FAILURE;
    }

    int server = socket(AF_INET, SOCK_STREAM, 0);
    char servermsg[255] = "Hello player!";

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portnumber);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&servaddr, sizeof servaddr) != 0) {
        printf("Bind failed!\n");
        return EXIT_FAILURE;
    }

    printf("Server is hosting on port: %d\n", ntohs(servaddr.sin_port));
    if (listen(server, 2) != 0) {
        fprintf(stderr, "Listen failed!\n");
        return EXIT_FAILURE;
    }

    int player1 = accept(server, NULL, NULL);
    send(player1, servermsg, sizeof servermsg, 0);
    int player2 = accept(server, NULL, NULL);
    send(player2, servermsg, sizeof servermsg, 0);

    int current_player;
    int other_player;

    char recv_buffer[255];
    int turn_count = 0;
    char send_buffer[255];
    while (turn_count < 200) {
        sprintf(send_buffer, "Your turn, player %d: ", (turn_count % 2) + 1);
        current_player = (turn_count % 2 == 0) ? player1 : player2;
        other_player = (current_player == player1) ? player2 : player1;
        send(current_player, send_buffer, sizeof send_buffer, 0);
        recv(current_player, recv_buffer, sizeof recv_buffer, 0);
        if (strcmp(recv_buffer, "done\n") == 0) {
            sprintf(send_buffer, "Connection closed by player %d\n", (turn_count % 2) + 1);
            send(other_player, send_buffer, sizeof send_buffer, 0);
            close(current_player);
            close(other_player);
            break;
        }
        memset(recv_buffer, 0, sizeof recv_buffer);
        // memset(message, 0, sizeof message);
        turn_count++;
    }
}
