// PrintFunction.cpp

#include "PrintFunction.h"

namespace Geometer
{
	wxIMPLEMENT_DYNAMIC_CLASS( PrintFunction, Function )
}

using namespace Geometer;

//========================================================================
PrintFunction::PrintFunction( void )
{
}

//========================================================================
/*virtual*/ PrintFunction::~PrintFunction( void )
{
}

//========================================================================
/*virtual*/ int PrintFunction::Call( lua_State* L )
{
	const char* string = luaL_checkstring( L, 1 );
	if( !string )
		return 0;

	while( true )
	{
		lua_getglobal( L, "console" );
		if( lua_istable( L, -1 ) )
			break;
		else
		{
			lua_pop( L, 1 );
			lua_newtable( L );
			lua_setglobal( L, "console" );
		}
	}

	while( true )
	{
		lua_pushstring( L, "output" );
		lua_gettable( L, -2 );
		if( lua_istable( L, -1 ) )
			break;
		else
		{
			lua_pop( L, 1 );
			lua_newtable( L );
			lua_setfield( L, -2, "output" );
		}
	}

	int length = lua_rawlen( L, -1 );
	lua_pushnumber( L, length + 1 );
	lua_pushvalue( L, 1 );
	lua_settable( L, -3 );
	return 0;
}

//========================================================================
/*virtual*/ wxString PrintFunction::LuaFunctionName( void )
{
	return "print";
}

// PrintFunction.cpp