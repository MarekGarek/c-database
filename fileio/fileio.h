#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>
#include <stdio.h>
#include "../structures/array_list.h"
#include "../user/user.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

_Bool load_registered_users(array_list* users);
void save_registered_user(const char* username, const char* password);
_Bool remove_registered_user(const char *username);
_Bool remove_directory(const char *path);

// create_user_table();
// delete_user_table();
// load_user_tables();
// add_user_table();
// load_table_content();
// save_table_content();

#endif //FILEIO_H