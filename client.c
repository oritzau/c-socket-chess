#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

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
}
