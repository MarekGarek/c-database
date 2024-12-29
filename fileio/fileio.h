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

_Bool load_registered_users(array_list* users);
void save_registered_user(const char* username, const char* password);

// _Bool create_user_table();
// _Bool delete_user_table();
//
// _Bool load_user_tables();
//
// _Bool add_user_table();
//
// _Bool load_table_content();
// _Bool save_table_content();

//**************** pomocné metódy

#endif //FILEIO_H


//Poznamky:
//              -   ukladat budem asi celu tabulku , teoreticky by slo pridat este
//                  dalsiu metodu na pridanie zaznamu na koniec suboru