cmake_minimum_required(VERSION 3.6)
add_definitions(-Wall -Wextra)

add_library(nettle
    sha256.c
    sha256-compress.c
    write-be32.c
    )


