#include "server_user.h"

static array_list registered_users;
static array_list logged_users;

void server_user_init() {
    array_list_init(&registered_users,sizeof(User*));
    array_list_init(&logged_users,sizeof(User*));

    load_registered_users(&registered_users);
    // if(is_user_registered("Marek14")) {
    //     printf("is registered - Marek14\n");
    // }
}

void server_user_destroy() {
     for (int i = 0; i < array_list_get_size(&registered_users); i++) {
         User* user = NULL;
         array_list_try_get(&registered_users,i,&user);
         if(user) free(user);
     }

    array_list_destroy(&registered_users);
    array_list_destroy(&logged_users);
}

bool login_user(const char *username, const char *password, int* token) {
    if(!username_exists(username)) {
        return false;
    }

    if(check_credentials(username, password)) {
        srand(time(NULL));
        *token = rand() % INT_MAX;
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->username,username);
        user->session_token = *token;
        //TODO FREE USER
        array_list_add(&logged_users, user); //&user
        return true;
    }

    return false;
}

bool register_user(const char *username, const char *password, int* token) {

}

//skontroluje ci uz username existuje
_Bool username_exists(const char *username) {
    for (int i = 0; i < array_list_get_size(&registered_users); i++) {
        User* user = NULL;
        array_list_try_get(&registered_users, i, &user);
        if (strcmp(user->username, username) == 0) {
            return true;
        }
    }
    return false;
}

//porovna meno a heslo
_Bool check_credentials(const char *username,const char* password) {
    for (int i = 0; i < array_list_get_size(&registered_users); i++) {
        User* user = NULL;
        array_list_try_get(&registered_users, i, &user);
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
            return true;
        }
    }
    return false;
}