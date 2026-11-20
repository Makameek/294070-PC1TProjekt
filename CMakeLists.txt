cmake_minimum_required(VERSION 3.8)
project(PC1TProjekt C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(PC1TProjekt
    PC1TProjekt.c
    tables.c
    reservations.c
    ui.c)

# Potlačení varování o "nebezpečných" funkcích (fscanf, sprintf, scanf...) v MSVC
target_compile_definitions(PC1TProjekt PRIVATE _CRT_SECURE_NO_WARNINGS)

# Šablona dne se zkopíruje vedle spustitelného souboru
configure_file(${CMAKE_SOURCE_DIR}/templateDay.txt
               ${CMAKE_BINARY_DIR}/templateDay.txt COPYONLY)
