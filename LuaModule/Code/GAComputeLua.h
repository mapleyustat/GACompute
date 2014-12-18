// GAComputeLua.h

#pragma once

#if defined EXPORT_GACOMPUTELUA
#	define GACOMPUTELUA_API		__declspec( dllexport )
#elif defined IMPORT_GACOMPUTELUA
#	define GACOMPUTELUA_API		__declspec( dllimport )
#else
#	define GACOMPUTELUA_API
#endif

#include <lua.hpp>

extern "C"
{
	extern GACOMPUTELUA_API int luaopen_gacomputelua( lua_State* L );
}

// GAComputeLua.h