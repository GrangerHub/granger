if(NOT WIN32)
    add_definitions(-Wall -Wextra)
endif(NOT WIN32)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_library(nettle
    bignum.c
    bignum.h
    gmp-glue.c
    gmp-glue.h
    macros.h
    md5.c
    md5-compress.c
    md5.h
    mini-gmp.c
    mini-gmp.h
    nettle-meta.h
    nettle-stdint.h
    nettle-types.h
    nettle-write.h
    pkcs1.c
    pkcs1.h
    pkcs1-rsa-sha256.c
    rsa.c
    rsa.h
    rsa-sha256-verify.c
    rsa-verify.c
    sha256.c
    sha256-compress.c
    sha2.h
    version.h
    write-be32.c
    write-le32.c
    )
