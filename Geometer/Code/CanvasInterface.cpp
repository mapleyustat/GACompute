// CanvasInterface.cpp

#include "CanvasInterface.h"

namespace Geometer
{
	wxIMPLEMENT_DYNAMIC_CLASS( CanvasInterface, Interface )
}

using namespace Geometer;

CanvasInterface::CanvasInterface( void )
{
}

/*virtual*/ CanvasInterface::~CanvasInterface( void )
{
}

/*virtual*/ void CanvasInterface::FillOutRegistryEntry( RegistryEntry& entry )
{
	entry.interfaceMenuItemName = "Canvas";
	entry.interfaceMenuItemHelp = "Toggle the canvas interface.";
}

/*virtual*/ bool CanvasInterface::CreateControls( void )
{
	return true;
}

/*virtual*/ bool CanvasInterface::UpdateControls( void )
{
	return true;
}

// CanvasInterface.cpp