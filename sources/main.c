#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct IpAddress
{
    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
};

int main(int argc, char **argv)
{
    int socket_desc;
    struct sockaddr_in server;
    char *message;

    char *host;
    int port;

    if (argc != 3)
    {
        printf("port argument missing");
        return 1;
    }
    host = argv[1];
    port = atoi(argv[2]);

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("socket creation error");
    }
    else
    {
        server.sin_addr.s_addr = inet_addr(host);
        server.sin_family = AF_INET;
        server.sin_port = htons((uint16_t)port);
        if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            printf("connection error\n");
            return 1;
        }
        else
        {
            struct IpAddress *ip = (struct IpAddress *)(&server.sin_addr.s_addr);

            message = "Oh, Hi Mark!";
            if (send(socket_desc, message, strlen(message), 0) < 0)
            {
                printf("send error\n");
            }

            message = malloc(1024);
            memset(message, 0, 1024);
            recv(socket_desc, message, 2000, 0);

            printf("%s:%d communicated via %d.%d.%d.%d:%hu -> %s\n", host, port, ip->byte0, ip->byte1, ip->byte2, ip->byte3, server.sin_port, message);
            free(message);
        }
    }
}
