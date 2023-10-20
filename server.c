#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main(void) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    char servermsg[255] = "Hello client!";

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9001);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&servaddr, sizeof servaddr) != 0) {
        printf("Bind failed!\n");
        return EXIT_FAILURE;
    }

    printf("Server is hosting on Port: %d\n", ntohs(servaddr.sin_port));
    if (listen(server, 2) != 0) {
        fprintf(stderr, "Listen failed!\n");
        return EXIT_FAILURE;
    }

    int client1 = accept(server, NULL, NULL);
    send(client1, servermsg, sizeof servermsg, 0);
    int client2 = accept(server, NULL, NULL);
    send(client2, servermsg, sizeof servermsg, 0);

    int current_client;


    char buff[255];
    int i = 0;
    char message[255];
    while (i < 20) {
        sprintf(message, "Your turn, client %d: ", i %2);
        current_client = (i % 2 == 0) ? client1 : client2;
        send(current_client, message, sizeof message, 0);
        recv(current_client, buff, sizeof buff, 0);
        printf("Message from client: %s", buff);
        if (strcmp(buff, "done\n") == 0)
            break;
        memset(buff, 0, sizeof buff);
        memset(message, 0, sizeof message);
        i++;
    }
}
