// Application.cpp

#include "Application.h"
#include "Interface.h"
#include "Function.h"

namespace Geometer
{
	wxIMPLEMENT_APP( Application );
}

using namespace Geometer;

Application::Application( void )
{
	frame = 0;

	L = 0;
}

/*virtual*/ Application::~Application( void )
{
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	Interface::PopulateRegistry();

	L = luaL_newstate();
	if( !L )
		return false;

	luaL_openlibs(L);
	
	Function::RegisterFunctions();

	frame = new Frame(0);
	frame->Show( true );

	return true;
}

/*virtual*/ int Application::OnExit( void )
{
	lua_close(L);

	return wxApp::OnExit();
}

// Application.cpp