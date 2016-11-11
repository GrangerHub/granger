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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "premake.h"

#if defined(PLATFORM_WINDOWS)
#include <shellapi.h>
static int do_elevate(lua_State *L)
{
	BOOL is_elevated = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			is_elevated = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	if (is_elevated) {
		return 1;
	}

	char szPath[MAX_PATH];
	if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {
		char *params = GetCommandLine();
		while (*params != '\0') {
			if (*params == ' ') {
				params++;
				break;
			}
			params++;
		}

		SHELLEXECUTEINFO sei = { 0 };
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.lpVerb = "runas";
		sei.lpFile = szPath;
		sei.lpParameters = params;
		sei.nShow = SW_NORMAL;

		if (ShellExecuteEx(&sei)) {
			exit(0);
		}
	}
	return 0;
}
#endif

#if defined(PLATFORM_MACOSX)
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>

static int do_elevate(lua_State *L)
{
	const char *execpath;
	char cwd[PATH_MAX];
	char *argv[2] = {0};

	if (geteuid() == 0) {
		return 1;
	}

	lua_getglobal(L, "_EXE_PATH");
	execpath = luaL_checkstring(L, 1);

	OSStatus err;
	AuthorizationRef ref;
	AuthorizationFlags flags;

	flags = kAuthorizationFlagDefaults;
	err = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, flags, &ref);

	if (err != errAuthorizationSuccess) {
		return 0;
	}

	AuthorizationItem _temp = {kAuthorizationRightExecute, 0, NULL, 0};
	AuthorizationRights rights = {1, &_temp};

	flags = kAuthorizationFlagDefaults |
			kAuthorizationFlagInteractionAllowed |
			kAuthorizationFlagPreAuthorize |
			kAuthorizationFlagExtendRights;

	err = AuthorizationCopyRights(ref, &rights, NULL, flags, NULL);
	if (err != errAuthorizationSuccess) {
		AuthorizationFree(ref, kAuthorizationFlagDefaults);
		return 0;
	}

	getcwd(cwd, sizeof(cwd));
	argv[0] = cwd;

	flags = kAuthorizationFlagDefaults;
	err = AuthorizationExecuteWithPrivileges(ref, execpath, flags, argv, NULL);
	AuthorizationFree(ref, kAuthorizationFlagDefaults);
	if (err != errAuthorizationSuccess) {
		return 0;
	}

	exit(0);
}
#endif

#if defined(PLATFORM_LINUX) || defined(PLATFORM_BSD)
static int do_elevate(lua_State *L)
{
	char *pkexec, *display;
	char cwd[PATH_MAX];
	int argc = 0;
	char *argv[6] = {0};

	if (geteuid() == 0) {
		return 1;
	}

	display = getenv("DISPLAY");
	if (!display) {
		return 0;
	}

	if (access("/usr/bin/pkexec", X_OK) != -1) {
		pkexec = "/usr/bin/pkexec";
	} else if (access("/usr/local/bin/pkexec", X_OK) != -1) {
		pkexec = "/usr/local/bin/pkexec";
	} else {
		return 0;
	}

	argv[argc++] = "pkexec";
	argv[argc++] = "--user";
	argv[argc++] = "root";

	lua_getglobal(L, "_EXE_PATH");
	argv[argc++] = (char *) luaL_checkstring(L, 1);

	getcwd(cwd, sizeof(cwd));
	argv[argc++] = cwd;

	execv(pkexec, argv);

	fprintf(stderr, "execv failed\n");
	lua_pop(L, 1);
	return 0;
}
#endif

int os_elevate(lua_State* L)
{
	lua_pushboolean(L, do_elevate(L));
	return 1;
}
