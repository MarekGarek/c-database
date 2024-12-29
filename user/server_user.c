#include "server_user.h"

static array_list registered_users;
static array_list logged_users;

void server_user_init() {
    array_list_init(&registered_users,sizeof(User*));
    array_list_init(&logged_users,sizeof(User*));

    load_registered_users(&registered_users);
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
    User* u = NULL;
    //username neexistuje alebo je uz prihlaseny
    if(find_logged_user(username,&u) || !username_exists(username)) {
        return false;
    }

    if(check_credentials(username, password)) {
        srand(time(NULL));
        *token = rand() % INT_MAX;
        User* user = (User*)malloc(sizeof(User));
        strcpy(user->username,username);
        strcpy(user->password,password);
        user->session_token = *token;
        array_list_add(&logged_users, &user);
        return true;
    }

    return false;
}

bool logout_user(int token, const char* username) {
    User* user = NULL;
    if(find_logged_user(username, &user)) {
        for(int i = 0; i < array_list_get_size(&logged_users); i++) {
            User* pomUser = NULL;
            array_list_try_get(&logged_users, i, &pomUser);
            if (strcmp(pomUser->username, username) == 0) {
                free(user);
                array_list_remove(&logged_users, i);
                return true;
            }
        }
    }
}

bool register_user(const char *username, const char *password, int* token) {

}

_Bool find_logged_user(const char * username, User** user) {
    if (array_list_get_size(&logged_users) != 0) {
        for (int i = 0; i < array_list_get_size(&logged_users); i++) {
            User* pomUser = NULL;
            array_list_try_get(&logged_users, i, &pomUser);
            if (strcmp(pomUser->username, username) == 0) {
                *user = pomUser;
                return true;
            }
        }
    }
    *user = NULL;
    return false;
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