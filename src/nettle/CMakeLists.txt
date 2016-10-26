if(NOT WIN32)
    add_definitions(-Wall -Wextra)
endif(NOT WIN32)

add_library(nettle
    macros.h
    nettle-stdint.h
    nettle-types.h
    nettle-write.h
    sha2.h
    sha256-compress.c
    sha256.c
    version.h
    write-be32.c
    )
