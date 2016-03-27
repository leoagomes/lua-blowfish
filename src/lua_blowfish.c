#include "lua_blowfish.h"

/**
	Blowfish main module.

	@classmod BFContext
*/

/**
	Encrypts a block.

	@function encrypt

	@string block a block of data to encrypt.
	@bool[opt=true] setPadding if true, sets the padded bytes to _paddingValue_
	@number[opt=0] paddingValue the value (between 0 and 255) to set the padding
	bytes value
	@treturn string the encrypted block
	@treturn ?number the amount of bytes used for padding

	@remarks
	Blowfish encrypts memory blocks 8 bytes at a time, so we need
	to pass an 8-byte aligned memory block and the returned block
	will also be aligned to 8 bytes. For that to be achieved, if 
	the given block's size is not a multiple of 8, it will be
	padded. Those bytes can either be ignored (will not be memset'd)
	or be set to a value between 0 and 255.
*/
static int lbf_encrypt (lua_State* L) {
	const char* data;
	size_t datalen;
	blowfish_ctx* ctx;
	int encdatasize, setpadding = 1, paddingval = 0, paddingc;

	ctx = luaL_checkudata (L, 1, "blowfish_meta");
	data = luaL_checklstring (L, 2, &datalen);

	paddingc = (datalen %8 == 0) ? 0 : 8 - (datalen % 8);
	encdatasize = datalen + paddingc;

	char* encdata = (char*) malloc (encdatasize * sizeof (char));
	throw_if_null (L, encdata, "encrypted data pointer");

	if (lua_gettop (L) >= 3)
		setpadding = luaL_checkinteger (L, 3);

	if (lua_gettop (L) >= 4)
		paddingval = luaL_checkinteger (L, 4);

	if (setpadding)
		memset (encdata+datalen, paddingval, paddingc);

	memcpy (encdata, data, datalen);

	blowfish_encrypt (ctx, datalen+paddingc, encdata, encdata);

	lua_pushlstring (L, encdata, encdatasize);
	lua_pushinteger (L, paddingc);

	return 2;
}

/**
	Decrypts a block.

	@function decrypt

	@string block block of data do decrypt

	@treturn string the decrypted block
	@treturn ?number the amount of bytes added for padding

	@remarks
	This function doesn't really expect data to be 8-byte aligned.
	It will allocate an 8-byte aligned block and copy encrypted data to that.
	It will decrypt that data and then return the block.
*/
static int lbf_decrypt (lua_State* L) {
	blowfish_ctx* ctx;
	const char* data;
	size_t datalen, paddingc;

	ctx = luaL_checkudata (L, 1, "blowfish_meta");
	data = luaL_checklstring (L, 2, &datalen);

	paddingc = (datalen % 8 == 0) ? 0 : 8 - (datalen % 8);

	char* decdata = (char*) calloc (datalen + paddingc, sizeof (char));
	throw_if_null (L, decdata, "Decrypted data pointer");

	memcpy (decdata, data, datalen);

	blowfish_decrypt (ctx, datalen+paddingc, decdata, decdata);

	lua_pushlstring (L, decdata, datalen + paddingc);
	lua_pushinteger (L, paddingc);

	return 2;
}

/*
	The following is the documentation for the function
	blowfish.create, but that shouldn't be treated inside the
	BFContext classmod. It is here because if anyone is reading
	this, this is the place it should go, but for ldoc to handle
	things properly, we're putting it inside lua_blowfish.h
*/
/**
	Creates a new instance˙of a blowfish cypher.

	@function create

	@string key the key to be used. It is from 4 to 112 bytes (or chars).

	@treturn BFContext a context to be used for encrypting/decrypting blocks	

	@local
*/
static int lbf_new (lua_State* L) {
	const char* key;
	size_t keylen;

	key = luaL_checklstring (L, 1, &keylen);

	blowfish_ctx* ctx = lua_newuserdata (L, sizeof (blowfish_ctx));

	blowfish_set_key (ctx, keylen, key);

	luaL_getmetatable (L, "blowfish_meta");

	lua_setmetatable (L, -2);

	return 1;
}

extern int luaopen_blowfish (lua_State* L) {
	luaL_Reg bf_m[] = {
		{"encrypt", lbf_encrypt},
		{"decrypt", lbf_decrypt},
		{NULL, NULL}
	};

	luaL_newmetatable (L, "blowfish_meta");
	luaL_register (L, NULL, bf_m);

	lua_pushvalue (L, -1);
	lua_setfield (L, -1, "__index");

	lua_pop (L, 1);

	lua_createtable (L, 0, 1);
	lua_pushcfunction (L, lbf_new);
	lua_setfield (L, -2, "create");

	return 1;
}
