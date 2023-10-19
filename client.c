#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int main(void) {
    int client = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9001);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client, (struct sockaddr *)(&servaddr), sizeof servaddr) < 0)
        printf("Connection error!\n");

    char buff[255];
    recv(client, buff, sizeof buff, 0);
    printf("Recieved message from server: %s\n", buff);

    char message[255];
    while (1) {
        printf("Send a message to the server: ");
        fgets(message, sizeof message, stdin);
        send(client, message, strlen(message), 0);
        if (strcmp(message, "done\n") == 0)
            break;
        memset(message, 0, sizeof message);
    }
}
