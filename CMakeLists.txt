cmake_minimum_required(VERSION 2.6)
project(tremulous-installer)
add_subdirectory(lua)
add_subdirectory(nettle)

add_executable(tremulous-installer
    lnettlelib.c
    main.c
    )

target_link_libraries(tremulous-installer lua nettle)

if (NOT WIN32)
    target_link_libraries(tremulous-installer m dl)
endif()
