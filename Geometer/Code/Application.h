// Application.h

#ifndef __Application_h__
#define __Application_h__

#include <wx/setup.h>
#include <wx/app.h>

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

#include "Frame.h"

namespace Geometer
{
	class Application;
}

//========================================================================
class Geometer::Application : public wxApp
{
public:

	Application( void );
	virtual ~Application( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;

	lua_State* LuaState( void ) { return L; }
	Frame* GetFrame( void ) { return frame; }

private:

	Frame* frame;

	// Implement undo/redo in a trivial way using the Lua state?
	lua_State* L;
};

//========================================================================
namespace Geometer
{
	wxDECLARE_APP( Application );
}

#endif __Application_h__

// Application.h