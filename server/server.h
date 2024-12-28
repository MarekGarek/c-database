#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../user/server_user.h"
#include "../fileio/fileio.h"
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

void start_server();
void *handle_client(void *client_socket);
void handle_login_register(char* buffer, char* response);
void parse_message(const char* message, char *parts[], int parts_count);

#endif // SERVER_H