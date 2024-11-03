#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[]) {    
    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[2]);
    int socket_fd, client_sock;
    struct sockaddr_in sock_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[1024];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);

    int returnval;

    returnval = bind(socket_fd,(struct sockaddr*)&sock_addr, sizeof(sock_addr));
    returnval  = listen(socket_fd, 5);

    printf("Echo server started on port %d. Waiting for a connection...\n", port);
    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
    
        client_sock = accept(socket_fd, (struct sockaddr *)&sock_addr, (socklen_t*)&client_addr_size);

        printf("Connected to client\n");

        ssize_t bytes_read;
        while ((bytes_read = read(client_sock, buffer, sizeof(buffer))) > 0) {
            printf("Received: %s", buffer);
            write(client_sock, buffer, bytes_read);
        }
        printf("Connection closed\n");
        break;
        close(client_sock);
    }
    return 0;
}
