#include "server.h"

volatile bool server_running = true;

void *console_listener(void *arg) {
    char command[20];
    while (server_running) {
        printf("[INFO] Zadaj 'exit' pre vypnutie servera: \n");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            command[strcspn(command, "\n")] = '\0';
            if (strcmp(command, "exit") == 0) {
                printf("[INFO] Prijaty prikaz na vypnutie servera.\n");
                server_running = false;
            }
        }
    }
    return NULL;
}

int main() {
    server_user_init();

    pthread_t console_thread;
    pthread_create(&console_thread, NULL, console_listener, NULL);

    start_server();

    pthread_join(console_thread, NULL);
    server_user_destroy();

    printf("[INFO] Server bol korektne vypnutý.\n");
    return 0;
}


void handle_login_register(char * buffer, char* response) {
    //printf("Správa od klienta: %s\n", buffer);
    char* parts[3];
    parse_message(buffer, parts, 3);

    int token = 0;
    if(strcmp(parts[0], "login") == 0) {
        if(login_user(parts[1], parts[2], &token)) {
            snprintf(response, 100, "SUCCESS:%d", token);
        } else {
            snprintf(response, 100, "FAIL");
        }
    } else if(strcmp(parts[0], "register") == 0) {
        if(register_user(parts[1], parts[2], &token)) {
            snprintf(response, 100, "SUCCESS:%d", token);
        } else {
            snprintf(response, 100, "FAIL");
        }
    }

    //printf("Response: %s\n", response);

    for (int i = 0; i < 3; i++) {
        free(parts[i]);
    }
}

void parse_message(const char *message, char *parts[], int parts_count) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s", message);

    int i = 0;
    char *token = strtok(buffer, ":");
    while (token != NULL && i < parts_count) {
        parts[i] = strdup(token);
        token = strtok(NULL, ":");
        i++;
    }

    if (i < parts_count) {
        parts[i] = NULL;
    }
}

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;
    char buffer[BUFFER_SIZE] = {0};

    printf("Klient pripojený, čakám na správy...\n");
    while (server_running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(socket, buffer, BUFFER_SIZE - 1);


        if (bytes_read <= 0 || strcmp(buffer, "exit") == 0) {
            printf("Klient ukončil spojenie alebo nastala chyba.\n");
            break;
        }

        char* response = malloc(100*sizeof(char));
        handle_login_register(buffer, response);

        if (send(socket, response, strlen(response), 0) < 0) {
            perror("Chyba pri odosielaní odpovede klientovi");
            break;
        }
        free(response);
    }

    close(socket);
    free(client_socket);
    printf("Spojenie s klientom ukončené.\n");
    return NULL;
}

//TODO momentálne 1 klient -> 1 vlákno , šlo by to prerobiť ideálne na nejaké asynchrónne I/O alebo to neviem...
void start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Server beží na porte %d, čakám na spojenia...\n", PORT);

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    while (server_running) {
        int addrlen = sizeof(address);
        int *client_socket = malloc(sizeof(int));
        if (!client_socket) {
            perror("Malloc failed");
            continue;
        }

        *client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (*client_socket < 0) {
            free(client_socket);
            usleep(100000);
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_socket) != 0) {
            perror("Failed to create thread");
            close(*client_socket);
            free(client_socket);
        }
        pthread_detach(thread_id);
    }

    close(server_fd);
    printf("[INFO] Server zastavený.\n");
}