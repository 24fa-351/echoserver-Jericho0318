#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* handleConnection(void* a_client_ptr) {
    int a_client = *(int*)a_client_ptr;
    free(a_client_ptr);

    while (1) {
        char buffer[1024];
        int bytes_read = read(a_client, buffer, sizeof(buffer));
        if (bytes_read == 0) {
            printf("Connection closed\n");
            close(a_client);
            break;
        }
        printf("Received: %s", buffer);
        write(a_client, buffer, bytes_read);
    }
}

int main(int argc, char const* argv[]) {
    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        printf("Usage: <./filename> -p <port>\n");
        exit(1);
    }

    int port = atoi(argv[2]);
    int socket_fd, client_sock;
    struct sockaddr_in sock_addr, client_addr;
    socklen_t client_addr_size;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(port);

    int returnval;

    returnval =
        bind(socket_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
    returnval = listen(socket_fd, 5);

    printf("Echo server started on port %d. Waiting for a connection...\n",
           port);
    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);

        client_sock = accept(socket_fd, (struct sockaddr*)&sock_addr,
                             (socklen_t*)&client_addr_size);
        if (client_sock == -1) {
            perror("Failed to accept.\n");
            exit(1);
        }

        printf("Connected to client\n");
        int* client_sock_ptr = (int*)malloc(sizeof(int));
        *client_sock_ptr = client_sock;

        pthread_t thread;
        pthread_create(&thread, NULL, handleConnection, (void*)client_sock_ptr);
    }
    return 0;
}
