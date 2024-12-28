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
_Bool register_user(const char* username, const char* password, int* token);

_Bool username_exists(const char *username);
_Bool check_credentials(const char* username, const char* password);


//komunikacia s klientom pre operacie login/register
// _Bool server_unregister();
// _Bool server_logout();
//
// //nacitanie a ulozenie registrovanych pouzivatelov
// void load_users();
// void save_users();
//
// //zoznam prihlasenych pouzivatelov
// void add_to_logged();
// void remove_from_logged();
// _Bool is_user_logged();
//
// //pomocne metody
// _Bool user_exists();
// void check_password();

#endif //SERVER_USER_H
