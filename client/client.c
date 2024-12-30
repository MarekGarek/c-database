#include "client.h"

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    sock = create_client_socket();
    connect_to_server(sock, &serv_addr);

    printf("Pripojený k serveru. Vyber operáciu:\n");
    bool connected = true;
    bool logged = false;
    char message[BUFFER_SIZE];
    bool exit = false;

    //credentials
    int session_token = 0;
    char login[20];
    bool success = false;

    while (connected) {

        if (!logged) {
            if (!login_register(sock, message, &exit, &session_token, login)) {
                if(exit) break;
            } else {
                printf("Vyber si operáciu: \n");
                fflush(stdout);
                logged = true;
            }
        }

        if (logged) {
            tab_logout_unregister(sock, &logged, message, session_token, login);
        }

        if (send(sock, "", 0, MSG_NOSIGNAL) == -1) {
            perror("[ERROR] Spojenie so serverom bolo prerušené");
            connected = false;
        }
    }

    close(sock);
    return 0;
}

bool login_register(int sock, char message[1024],bool* exit,int* session_token, char* login) {
    char operation[20];
    char pwd[20];
    char pwd2[20];
    bool success = false;

    printf("'exit' 'login' 'register'\n");
    fgets(operation, 20, stdin);
    operation[strcspn(operation, "\n")] = '\0';

    if (strcmp(operation, "exit") == 0) {
        printf("Ukončujem spojenie so serverom...\n");
        *exit = true;
        return false;
    } else if (strcmp(operation, "login") == 0) {
        printf("Zadaj login: \n");
        fgets(login, 20, stdin);
        printf("Zadaj heslo: \n");
        fgets(pwd, 20, stdin);
        login[strcspn(login, "\n")] = '\0';
        pwd[strcspn(pwd, "\n")] = '\0';

        char pwd_h[65];
        hash_pwd(pwd,pwd_h);
        sprintf(message, "%s:%s:%s", operation,login,pwd_h);
        send_message(sock, message);

        char* response = receive_message(sock);
        parse_response(response,&success,session_token);

        if (success) {
            printf("Úspešne si sa prihlásil %s! session_token: %d \n",login,*session_token);
            free(response);
            return true;
        }
        printf("Login neúspešný!\n");
        free(response);
        return false;
    } else if (strcmp(operation, "register") == 0) {
        printf("Zadaj login: \n");
        fgets(login, 20, stdin);
        printf("Zadaj heslo: \n");
        fgets(pwd, 20, stdin);
        printf("Zopakuj heslo: \n");
        fgets(pwd2, 20, stdin);
        if(strcmp(pwd,pwd2) == 0) {
            login[strcspn(login, "\n")] = '\0';
            pwd[strcspn(pwd, "\n")] = '\0';

            char pwd_h[65];
            hash_pwd(pwd,pwd_h);
            sprintf(message, "%s:%s:%s", operation,login,pwd_h);
            printf("%s\n",message);
            send_message(sock, message);

            char* response = receive_message(sock);
            parse_response(response,&success,session_token);

            if (success) {
                printf("Úspešne si sa zaregistroval %s! session_token: %d \n",login,*session_token);
                free(response);
                return true;
            }
            printf("Register neúspešný! Login je obsadený.\n");
            free(response);
            return false;
        } else {
            printf("Heslá sa nezhodujú!\n");
        }
    }
    return false;
}

_Bool tab_logout_unregister(int sock, bool *logged, char message[1024], int session_token, char login[20]) {
    bool success = false;
    char operation[20];
    printf("'tab' 'logout' 'unregister'\n");
    fgets(operation, BUFFER_SIZE, stdin);
    operation[strcspn(operation, "\n")] = '\0';
    if (strcmp(operation, "tab") == 0) {
        //TODO vypíše userove tabuľky + operácie
        // 'new_tab' 'delete_tab' 'open_tab' 'go_back'
        //   |_ 'name'                |_ 'modify' 'show'
        //   |_ 'columns'                   |       |_ 'show' 'order by' 'contain_str'
        //   |_ 'go_back'                   |
        //                                  |_ 'add_record' 'delete_record' 'save'
    } else if (strcmp(operation, "logout") == 0) {
        sprintf(message, "%s:%d:%s", operation,session_token,login);
        //printf("[DEBUG] message for logout: %s\n", message);
        send_message(sock, message);

        char* response = receive_message(sock);
        parse_response(response,&success,&session_token);

        if (success) {
            printf("Úspešne si sa odhlásil!\n");
            free(response);
            *logged = false;
            return true;
        }
        printf("Logout neúspešný!\n");
        free(response);
        return false;
    } else if (strcmp(operation, "unregister") == 0) {
        sprintf(message, "%s:%d:%s", operation,session_token,login);
        send_message(sock, message);

        char* response = receive_message(sock);
        parse_response(response,&success,&session_token);

        if (success) {
            printf("Úspešne si sa odregistroval!\n");
            free(response);
            *logged = false;
            return true;
        }
        printf("Unregister neúspešný!\n");
        free(response);
        return false;
    }
    return false;
}

void parse_response(const char* response, bool* success, int* session_token) {
    char buffer[256];
    strncpy(buffer, response, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    char* token = strtok(buffer, ":");

    if (strcmp(token, "FAIL") == 0) {
        *success = false;
        *session_token = -1;
        return;
    }

    if (strcmp(token, "SUCCESS") == 0) {
        *success = true;
        token = strtok(NULL, ":");
        if (token != NULL) {
            *session_token = atoi(token);
        } else {
            *session_token = -1;
        }
        return;
    }

    *success = false;
    *session_token = -1;
}


void hash_pwd(const char *input, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

// Vytvorenie socketu
int create_client_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    return sock;
}

// Pripojenie k serveru
void connect_to_server(int sock, struct sockaddr_in *serv_addr) {
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

// Odoslanie správy serveru
void send_message(int sock, const char *message) {
    send(sock, message, strlen(message), 0);
    //printf("Odoslaná správa: %s\n", message);
}

// Prijatie a spracovanie spravy zo servera, vrati odpoved
char* receive_message(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    read(sock, buffer, BUFFER_SIZE);
    char *message = malloc(strlen(buffer) + 1);
    strcpy(message, buffer);
    return message;
}