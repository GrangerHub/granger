/*
 * This file is part of Granger.
 * Copyright (c) 2015 Jeff Kent <jeff@jkent.net>
 *
 * Granger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Granger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Granger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "premake/premake.h"

#include "lnettlelib.h"

extern const luaL_Reg extra_os_functions[];

int main (int argc, char *argv[])
{
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

	premake_init(L);
	premake_locate(L, argv[0]);
	lua_setglobal(L, "_EXE_PATH");

	if (argc > 1) {
		if (chdir(argv[1])) {
			fprintf(stderr, "Error: unable to chdir\n");
			lua_close(L);
			return EXIT_FAILURE;
		}
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
