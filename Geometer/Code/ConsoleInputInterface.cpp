// ConsoleInputInterface.cpp

#include "ConsoleInputInterface.h"
#include "Application.h"

#include <wx/msgdlg.h>
#include <wx/button.h>

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

namespace Geometer
{
	wxIMPLEMENT_DYNAMIC_CLASS( ConsoleInputInterface, Interface )
}

using namespace Geometer;

ConsoleInputInterface::ConsoleInputInterface( void )
{
}

/*virtual*/ ConsoleInputInterface::~ConsoleInputInterface( void )
{
}

/*virtual*/ void ConsoleInputInterface::FillOutRegistryEntry( RegistryEntry& entry )
{
	entry.interfaceMenuItemName = "Console Input";
	entry.interfaceMenuItemHelp = "Toggle the conole input interface.";
}

/*virtual*/ bool ConsoleInputInterface::CreateControls( void )
{
	textControl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER );

	wxButton* executeButton = new wxButton( this, wxID_ANY, "Execute" );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( textControl, 1, wxGROW );
	boxSizer->Add( executeButton, 0, wxGROW );
	SetSizer( boxSizer );

	// A button should not be a bad way to go, because the user should
	// be able to press TAB-ENTER.  The button will then give the focus
	// back to the text control.
	Bind( wxEVT_BUTTON, &ConsoleInputInterface::OnExecute, this );

	return true;
}

/*virtual*/ bool ConsoleInputInterface::UpdateControls( void )
{
	return true;
}

void ConsoleInputInterface::OnExecute( wxCommandEvent& event )
{
	lua_State* L = wxGetApp().LuaState();

	wxString text = textControl->GetValue();
	int result = luaL_loadbuffer( L, text, text.Length(), "consoleInputChunk" );
	if( result == LUA_ERRSYNTAX )
	{
		wxString error = lua_tostring( L, -1 );
		wxString message = "Geometer console input: " + error;
		wxMessageDialog messageDialog( wxGetApp().GetFrame(), message, "Lua Syntax Error", wxOK | wxCENTRE | wxICON_ERROR );
		messageDialog.ShowModal();
		lua_pop( L, 1 );
		return;
	}

	textControl->Clear();

	result = lua_pcall( L, 0, 0, 0 );
	if( result == LUA_ERRRUN )
	{
		wxString error = lua_tostring( L, -1 );
		wxString message = "Geometer console input: " + error;
		wxMessageDialog messageDialog( wxGetApp().GetFrame(), message, "Lua Run-time Error", wxOK | wxCENTRE | wxICON_ERROR );
		messageDialog.ShowModal();
		lua_pop( L, 1 );
		return;
	}

	textControl->SetFocus();

	// TODO: Concatinate to console.input.

	TouchedLuaState( false );
}

// ConsoleInputInterface.cpp