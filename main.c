#include <stdio.h>
#include <stdlib.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "lnettlelib.h"

int main (int argc, char *argv[])
{
	int result;
	lua_State *L;

	L = luaL_newstate();
	if (L == NULL) {
		fprintf(stderr, "cannot create lua state\n");
		return EXIT_FAILURE;
	}

	luaL_openlibs(L);
	luaL_requiref(L, "nettle", luaopen_nettle, 1);
	lua_pop(L, 1);

	if (luaL_dofile(L, "main.lua")) {
		fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
		lua_close(L);
		return EXIT_FAILURE;
	}

	lua_close(L);
	return EXIT_SUCCESS;
}
