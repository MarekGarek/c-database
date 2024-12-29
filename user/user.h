#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    char username[100];
    char password[100];
    int session_token;
} User;

User* user_init(char* username, char* password, int token);
void user_destroy(User* user);
_Bool user_validate_token(User* user, int token);

#endif //USER_H
