#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>


int main(void) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    char servermsg[255] = "Yo what's up client";

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9001);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&servaddr, sizeof servaddr) != 0)
        printf("Bind failed!\n");

    listen(server, 1);

    int client = accept(server, NULL, NULL);

    send(client, servermsg, sizeof servermsg, 0);
}
