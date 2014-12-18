// NewUserData.cpp

#include "NewUserData.h"
#include "UserData.h"

int l_newscalar( lua_State* L )
{
	int stackTop = lua_gettop( L );
	if( stackTop < 1 )
		return luaL_error( L, "Expected an argument." );

	if( lua_isnumber( L, -1 ) )
	{
		double number = lua_tonumber( L, -1 );
		lua_remove( L, -1 );

		UserData* userData = l_newuserdata( L );
		if( !userData )
			return 0;

		userData->multivector->AssignScalar( number );
		return 1;
	}
	else if( lua_isstring( L, -1 ) )
	{
		const char* scalarVariableName = lua_tostring( L, -1 );
		lua_remove( L, -1 );

		UserData* userData = l_newuserdata( L );
		if( !userData )
			return 0;

		userData->multivector->AssignVector( scalarVariableName );
		return 1;
	}
	
	return luaL_error( L, "Expected a number or string argument." );
}

int l_newvector( lua_State* L )
{
	int stackTop = lua_gettop( L );
	if( stackTop < 1 )
		return luaL_error( L, "Expected an argument." );

	if( !lua_isstring( L, -1 ) )
	{
		lua_remove( L, -1 );
		return luaL_error( L, "Expected a string argument." );
	}

	const char* vectorName = lua_tostring( L, -1 );
	lua_remove( L, -1 );

	UserData* userData = l_newuserdata( L );
	if( !userData )
		return 0;

	userData->multivector->AssignVector( vectorName );
	return 1;
}

// NewUserData.cpp