// chat_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void *receive_messages(void *socket);

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    pthread_t thread_id;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect error");
        exit(1);
    }

    pthread_create(&thread_id, NULL, receive_messages, (void *)&client_socket);

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';  // Remove newline character
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}

void *receive_messages(void *socket) {
    int client_socket = *(int *)socket;
    char buffer[BUFFER_SIZE];
    int len;

    while ((len = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[len] = '\0';
        printf("%s\n", buffer);
    }

    return NULL;
}
