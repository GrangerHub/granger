#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "lnettlelib.h"

int main (int argc, char *argv[])
{
	int result;
	char *main_path = ".";
	char *main_lua;
	lua_State *L;

	L = luaL_newstate();
	if (L == NULL) {
		fprintf(stderr, "cannot create lua state\n");
		return EXIT_FAILURE;
	}

	luaL_openlibs(L);
	luaL_requiref(L, "nettle", luaopen_nettle, 1);
	lua_pop(L, 1);

	if (argc > 1) {
		const char *path;
		char *new_path;

		main_path = argv[1];
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path");
		path = lua_tostring(L, -1);
		new_path = malloc(strlen(path) + strlen(main_path) + 8);
		sprintf(new_path, "%s;%s%s?.lua", path, main_path, LUA_DIRSEP);
		lua_pop(L, 1);
		lua_pushstring(L, new_path);
		free(new_path);
		lua_setfield(L, -2, "path");
		lua_pop(L, 1);
	}

	main_lua = malloc(strlen(main_path) + 10);
	sprintf(main_lua, "%s%smain.lua", main_path, LUA_DIRSEP);
	if (luaL_dofile(L, main_lua)) {
		fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
		free(main_lua);
		lua_close(L);
		return EXIT_FAILURE;
	}

	free(main_lua);
	lua_close(L);
	return EXIT_SUCCESS;
}
