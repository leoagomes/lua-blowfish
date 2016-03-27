#pragma once

#include "lua5.1/lua.h"
#include "lua5.1/lauxlib.h"
#include "blowfish.h"

#include <string.h>
#include <stdlib.h>

#define throw_if_null(L,ptr,ptrname) \
	if (!(ptr)) \
		return luaL_error ((L), "%s is null. Failed.", (ptrname));

typedef struct blowfish_ctx blowfish_ctx;

extern int luaopen_blowfish (lua_State* L);

/*
	This part is just for making ldoc handle everything properly.
	It can and should be ignored. This piece of documentation is
	also available at lua_blowfish.c
*/

/**
	Blowfish main module.

	@module blowfish
*/

/**
	Creates a new instance˙of a blowfish cypher.

	@function create

	@string key the key to be used. It is from 4 to 112 bytes (or chars).

	@treturn BFContext a context to be used for encrypting/decrypting blocks	
*/
