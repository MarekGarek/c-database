#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    char username[20];
    char password[20];
    int session_token;
} User;

User* user_init(char* username, char* password, int token);
void user_destroy(User* user);
_Bool user_validate_token(User* user, int token);

#endif //USER_H
