#include "premake.h"

#if defined(PLATFORM_WINDOWS)
#include <io.h>
#define access _access
#define X_OK 1
#define W_OK 2
#define R_OK 4
#endif

int os_access(lua_State *L)
{
	const char *filename = luaL_checkstring(L, 1);
	const char *mode = luaL_optstring(L, 2, "r");
	int nmode = 0;
	int result;

	while (*mode) {
		switch (*mode) {
		case 'x':
			nmode |= X_OK;
			break;
		case 'w':
			nmode |= W_OK;
			break;
		case 'r':
			nmode |= R_OK;
			break;
		default:
			lua_pushboolean(L, 0);
			return 1;
		}
		mode++;
	}

	result = access(filename, nmode);

	lua_pushboolean(L, result == 0);
	return 1;
}
