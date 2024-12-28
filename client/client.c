#include "client.h"

// Hlavná funkcia
int main() {
    int sock;
    struct sockaddr_in serv_addr;
    sock = create_client_socket();
    connect_to_server(sock, &serv_addr);

    printf("Pripojený k serveru. Vyber operáciu:\n");
    bool connected = true;

    char message[BUFFER_SIZE];
    char operation[10];
    char login[20];
    char pwd[20];
    char pwd2[20];

    //TODO refactor do login_register metódy
    while (connected) {
        printf("'exit' 'login' 'register'\n");
        fgets(operation, BUFFER_SIZE, stdin);
        operation[strcspn(operation, "\n")] = '\0';

        if (strcmp(operation, "exit") == 0) {
            printf("Ukončujem spojenie so serverom...\n");
            break;
        } else if (strcmp(operation, "login") == 0) {
            printf("Zadaj login: \n");
            fgets(login, BUFFER_SIZE, stdin);
            printf("Zadaj heslo: \n");
            fgets(pwd, BUFFER_SIZE, stdin);

            login[strcspn(login, "\n")] = '\0';
            pwd[strcspn(pwd, "\n")] = '\0';
            //TODO pwd zahashovať pred odoslaním
            sprintf(message, "%s:%s:%s", operation,login,pwd);
            send_message(sock, message);




            char* response = receive_message(sock);
            printf("%s\n", response);
            //TODO
            // parse response token ulozit




            if (strcmp(response,"SUCCESS") == 0) {
                printf("Login úspešný!\n");
                //TODO logika pracovania na serveri / dalsi nekonecny while
            } else if (strcmp(response,"FAIL") == 0) {
                printf("Login neúspešný!\n");
            }
            free(response);
        } else if (strcmp(operation, "register") == 0) {
            printf("Zadaj login: \n");
            fgets(login, BUFFER_SIZE, stdin);
            printf("Zadaj heslo: \n");
            fgets(pwd, BUFFER_SIZE, stdin);
            printf("Zopakuj heslo: \n");
            fgets(pwd2, BUFFER_SIZE, stdin);
            if(strcmp(pwd,pwd2) == 0) {
                login[strcspn(login, "\n")] = '\0';
                pwd[strcspn(pwd, "\n")] = '\0';

                //TODO pwd zahashovať pred odoslaním
                sprintf(message, "%s:%s:%s", operation,login,pwd);
                send_message(sock, message);

                char* response = receive_message(sock);
                if (strcmp(response,"SUCCESS") == 0) {
                    printf("Registrácia prebehla úspešne!\n");
                    //TODO po registrácii bude užívateľ prihlásený do systému
                    //TODO logika pracovania na serveri / dalsi nekonecny while
                } else if (strcmp(response,"FAIL") == 0) {
                    printf("Používateľ s daným loginom už existuje, skúste iný.\n");
                }
                free(response);
            } else {
                printf("Heslá sa nezhodujú!\n");
            }
        }

        if (send(sock, "", 0, MSG_NOSIGNAL) == -1) {
            perror("[ERROR] Spojenie so serverom bolo prerušené");
            connected = false;
        }
    }

    close(sock);
    return 0;
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