#ifndef SERVER_USER_H
#define SERVER_USER_H

#include "user.h"
#include <limits.h>
#include <time.h>
#include "../structures/array_list.h"
#include "../fileio/fileio.h"

void server_user_init();
void server_user_destroy();

_Bool login_user(const char* username, const char* password, int* token);
_Bool logout_user(int token, const char* username);
_Bool register_user(const char* username, const char* password, int* token);
_Bool unregister_user(int token, const char* username);
_Bool validate_token(int token, const char* username);

_Bool find_logged_user(const char * username,User** user);
_Bool username_exists(const char *username);
_Bool check_credentials(const char* username, const char* password);
_Bool remove_user(const char* username, array_list* array);

#endif //SERVER_USER_H