#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>
#include <stdio.h>
#include "../structures/array_list.h"
#include "../user/user.h"

_Bool load_registered_users(array_list* users);

// _Bool save_registered_users();
//
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