// ConsoleOutputInterface.cpp

#include "ConsoleOutputInterface.h"
#include "Application.h"

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

namespace Geometer
{
	wxIMPLEMENT_DYNAMIC_CLASS( ConsoleOutputInterface, Interface )
}

using namespace Geometer;

//========================================================================
ConsoleOutputInterface::ConsoleOutputInterface( void )
{
	linesRead = 0;
}

//========================================================================
/*virtual*/ ConsoleOutputInterface::~ConsoleOutputInterface( void )
{
}

//========================================================================
/*virtual*/ void ConsoleOutputInterface::FillOutRegistryEntry( RegistryEntry& entry )
{
	entry.interfaceMenuItemName = "Console Output";
	entry.interfaceMenuItemHelp = "Toggle the conole output interface.";
}

//========================================================================
/*virtual*/ bool ConsoleOutputInterface::CreateControls( void )
{
	textControl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( textControl, 1, wxGROW );
	SetSizer( boxSizer );

	return true;
}

//========================================================================
/*virtual*/ bool ConsoleOutputInterface::UpdateControls( void )
{
	bool updated = false;
	lua_State* L = wxGetApp().LuaState();
	int top = lua_gettop( L );

	do
	{
		lua_getglobal( L, "console" );
		if( !lua_istable( L, -1 ) )
			break;
	
		lua_getfield( L, -1, "output" );
		if( !lua_istable( L, -1 ) )
			break;

		int length = lua_rawlen( L, -1 );
		if( linesRead == length )
		{
			updated = true;
			break;
		}

		linesRead++;
		lua_pushnumber( L, linesRead );
		lua_gettable( L, -2 );
		const char* line = lua_tostring( L, -1 );
		if( line )
		{
			wxString text = line;
			text += "\n";
			textControl->AppendText( text );
		}
	}
	while( false );

	while( lua_gettop( L ) > top )
		lua_pop( L, 1 );

	return updated;
}

// ConsoleOutputInterface.cpp