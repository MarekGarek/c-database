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

bool remove_registered_user(const char *username) {
    // remove from registered_users
    char* path = "/home/ubuntu/c-projects/semestralka/data/registered_users.txt";
    char* tempPath = "/home/ubuntu/c-projects/semestralka/data/temp.txt";

    FILE* f = fopen(path,"r");
    if (!f) {
        perror("Failed to open file with registered users.\n");
        return false;
    }

    FILE* temp = fopen(tempPath,"w");
    if (!temp) {
        perror("Failed to open file with temp.txt.\n");
        fclose(f);
        return false;
    }

    char line[256];
    char login[100];
    char pwd[100];

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%99[^:]:%99[^\n]", login, pwd) == 2) {
            if (strcmp(login, username) == 0) {
                continue;
            }
        }
        fputs(line, temp);
    }
    fclose(f);
    fclose(temp);

    char pathDir[256] = "/home/ubuntu/c-projects/semestralka/data/users/";
    strcat(pathDir, username);
    return (remove(path) == 0) && (rename(tempPath, path) == 0) && remove_directory(pathDir);
}

bool remove_directory(const char *path) {
    struct dirent *entry;
    char full_path[1024];
    DIR *dir = opendir(path);

    if (!dir) return false;

    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            remove_directory(full_path);
        } else {
            unlink(full_path);
        }
    }
    closedir(dir);
    return rmdir(path) == 0;
}