#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char ** argv) {
    int client = socket(AF_INET, SOCK_STREAM, 0);

    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char * server_ip = argv[1];
    int server_port = atoi(argv[2]);
    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    servaddr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(client, (struct sockaddr *)(&servaddr), sizeof servaddr) < 0)
        printf("Connection error!\n");

    char buff[255];
    recv(client, buff, sizeof buff, 0);
    printf("Recieved message from server: %s\n", buff);

    char message[255];
    char buffer[255];
    while (1) {
        recv(client, buffer, sizeof buff, 0);
        printf("%s", buffer);
        printf("Send a message to the server: ");
        fgets(message, sizeof message, stdin);
        send(client, message, strlen(message), 0);
        if (strcmp(message, "done\n") == 0)
            break;
        memset(message, 0, sizeof message);
    }
}
