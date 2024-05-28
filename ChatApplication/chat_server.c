// chat_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *client_socket);
void send_message_to_all(char *message, int sender_socket);

int main() {
    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen error");
        exit(1);
    }

    printf("Server is listening on port 8080\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);

        if (client_socket < 0) {
            perror("Accept error");
            exit(1);
        }

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_socket;
                pthread_create(&thread_id, NULL, handle_client, (void *)&client_sockets[i]);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;
    char buffer[BUFFER_SIZE];
    int len;

    while ((len = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[len] = '\0';
        send_message_to_all(buffer, socket);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == socket) {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    close(socket);
    return NULL;
}

void send_message_to_all(char *message, int sender_socket) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_socket) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}
