#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <openssl/sha.h>


#define PORT 8080
#define BUFFER_SIZE 1024

// Deklarácie funkcií
int create_client_socket();
void connect_to_server(int sock, struct sockaddr_in *serv_addr);
void send_message(int sock, const char *message);
char* receive_message(int sock);
_Bool login_register(int sock, char message[1024],bool* exit,int* session_token, char* login);
_Bool tab_logout_unregister(int sock, bool *logged, char message[1024], int session_token, char login[20]);
void parse_response(const char* response, bool* success, int* session_token);
void hash_pwd(const char* input, char* output);

// void user_register();
// void user_unregister();
// void user_login();
// void user_logout();

#endif // CLIENT_H
