#include "fileio.h"

bool load_registered_users(array_list* users) {
    FILE* f = fopen("/home/ubuntu/c-projects/semestralka/data/registered_users.txt", "r");
    if (f == NULL) {
        perror("Failed to open file with registered users.\n");
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, ":");
        if (token != NULL) {
            User* user = (User*)malloc(sizeof(User));
            if (user == NULL) {
                perror("Failed to allocate memory for User.\n");
                fclose(f);
                return false;
            }

            user->session_token = 0;
            strncpy(user->username, token, sizeof(user->username) - 1);
            user->username[sizeof(user->username) - 1] = '\0';

            token = strtok(NULL, ":");
            if (token != NULL) {
                strncpy(user->password, token, sizeof(user->password) - 1);
                user->password[sizeof(user->password) - 1] = '\0';
            }

            array_list_add(users, &user);
            //printf("Username='%s', Password='%s'\n", user->username, user->password);
        }
    }
    fclose(f);
    return true;
}

void save_registered_user(const char *username, const char *password) {
    FILE* f = fopen("/home/ubuntu/c-projects/semestralka/data/registered_users.txt", "a");
    if (f == NULL) {
        perror("Failed to open file with registered users.\n");
    }
    fprintf(f, "%s:%s\n", username,password);
    fclose(f);
    char path[1024] = "/home/ubuntu/c-projects/semestralka/data/users/";
    strcat(path, username);
    mkdir(path,0755);
}
