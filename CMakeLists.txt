cmake_minimum_required(VERSION 3.16)
project(semestralka C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

set(COMMON_SOURCES
        structures/linked_list.c
        structures/linked_list.h
        structures/array_list.c
        structures/array_list.h
        fileio/fileio.c
        fileio/fileio.h
        user/server_user.c
        user/server_user.h
        table/table.c
        table/table.h
        user/user.h
        user/user.c
)

# Server
add_executable(server
        ${COMMON_SOURCES}
        server/server.c
)
target_link_libraries(server Threads::Threads)
# Client 1
add_executable(client1
        ${COMMON_SOURCES}
        client/client.c
)
target_link_libraries(client1 Threads::Threads)
# Client 2
add_executable(client2
        ${COMMON_SOURCES}
        client/client.c
)
target_link_libraries(client2 Threads::Threads)
# Client 3
add_executable(client3
        ${COMMON_SOURCES}
        client/client.c
)
target_link_libraries(client3 Threads::Threads)
