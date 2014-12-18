// Version.cpp

#include "Version.h"

int l_version( lua_State* L )
{
	float major = float( GACOMPUTELUA_VERSION_MAJOR );
	float minor = float( GACOMPUTELUA_VERSION_MINOR );

	float version = major + minor / 10.f;

	lua_pushnumber( L, version );
	return 1;
}

// Version.cpp