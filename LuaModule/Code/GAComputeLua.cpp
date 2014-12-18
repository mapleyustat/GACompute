// GAComputeLua.cpp

#include "GAComputeLua.h"
#include "Version.h"
#include "NewUserData.h"
#include "BilinearForm.h"

static luaL_Reg gacomputelua_api[] =
{
	{ "version", l_version },
	{ "newscalar", l_newscalar },
	{ "newvector", l_newvector },
	{ "bilinearform", l_bilinearform },
	{ NULL, NULL },
};

int luaclose_gacomputelua( lua_State* L )
{
	return 0;
}

extern "C"
{
	GACOMPUTELUA_API int luaopen_gacomputelua( lua_State* L )
	{
		luaL_newlib( L, gacomputelua_api );

		lua_newtable( L );
		lua_pushcfunction( L, &luaclose_gacomputelua );
		lua_setfield( L, -2, "__gc" );
		lua_setmetatable( L, -2 );

		return 1;
	}
}

// GAComputeLua.cpp