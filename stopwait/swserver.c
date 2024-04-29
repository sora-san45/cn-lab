#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
    int server, newSock, k = 5, m = 1, p;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;
    server = socket(PF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5600);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));

    bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr));

    if (listen(server, 5) == 0)
        printf("Listening\n");
    else {
        printf("Error\n");
        exit(1);
    }

    addrSize = sizeof(store);
    newSock = accept(server, (struct sockaddr*)&store, &addrSize);

    if (newSock == -1) {
        printf("Error in creating socket\n");
        exit(1);
    }

    while (k != 0) {
        int y = recv(newSock, buffer, 1024, 0);

        if (y == -1) {
            printf("Error in receiving\n");
            exit(1);
        }

        if (strncmp(buffer, "frame", 5) == 0)
            printf("Received %d successfully \n", m);
        else
            printf("Frame %d not received\n", m);

        if (m % 2 == 0)
            strcpy(buffer, "ack");
        else {
            strcpy(buffer, "kca");
            printf("Ack lost\n");
            for (p = 1; p <= 3; p++) {
                printf("Waiting for %d seconds\n", p);
                sleep(1);
            }
            printf("Retransmitting ack... \n");
            strcpy(buffer, "ack");
        }

        printf("Sending ack %d...\n", m);
        int z = send(newSock, buffer, 19, 0);

        if (z == -1) {
            printf("Error in sending\n");
            exit(1);
        }
        k--;
        m++;
    }
    close(newSock);
}

