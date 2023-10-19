#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

int main(void) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    char servermsg[255] = "Yo what's up client";

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9001);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server, (struct sockaddr *)&servaddr, sizeof servaddr) != 0) {
        printf("Bind failed!\n");
        return EXIT_FAILURE;
    }

    listen(server, 1);

    int client1 = accept(server, NULL, NULL);
    int client2 = accept(server, NULL, NULL);

    int current_client;

    send(client1, servermsg, sizeof servermsg, 0);
    send(client2, servermsg, sizeof servermsg, 0);

    char buff[255];
    int i = 0;
    while (i < 20) {
        current_client = (i % 2 == 0) ? client1 : client2;
        recv(current_client, buff, sizeof buff, 0);
        printf("Message from client: %s", buff);
        if (strcmp(buff, "done\n") == 0)
            break;
        memset(buff, 0, sizeof buff);
    }
}
