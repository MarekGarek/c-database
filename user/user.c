#include "user.h"

User * user_init(char *username, char *password, int token) {
    User *user = (User *)malloc(sizeof(User));

    strncpy(user->username, username, sizeof(user->username) - 1);
    user->username[sizeof(user->username) - 1] = '\0';

    strncpy(user->password, password, sizeof(user->password) - 1);
    user->password[sizeof(user->password) - 1] = '\0';

    user->session_token = token;
    return user;
}

void user_destroy(User *user) {
    if (user) free(user);
}

_Bool user_validate_token(User *user, int token) {
    if (user && user->session_token == token) {
        return true;
    }
    return false;
}