add_subdirectory(lua)
add_subdirectory(nettle)
add_subdirectory(premake)

find_library(CORESERVICES NAMES CoreServices)

add_executable(tremulous-installer
    lnettlelib.c
    main.c
    )

target_link_libraries(tremulous-installer lua nettle premake)

if (NOT WIN32)
    target_link_libraries(tremulous-installer m dl)
endif()

if (APPLE)
	macro(ADD_FRAMEWORK fwname appname)
		find_library(FRAMEWORK_${fwname}
	    NAMES ${fwname}
	    PATHS ${CMAKE_OSX_SYSROOT}/System/Library
	    PATH_SUFFIXES Frameworks
	    NO_DEFAULT_PATH)
	    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
	        MESSAGE(ERROR ": Framework ${fwname} not found")
	    else()
	        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
	        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
	    endif()
	endmacro(ADD_FRAMEWORK)

	add_framework(CoreServices tremulous-installer)
endif()

