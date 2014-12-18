// CanvasInterface.h

#ifndef __CanvasInterface_h__
#define __CanvasInterface_h__

#include "Interface.h"

namespace Geometer
{
	class CanvasInterface;
}

//========================================================================
class Geometer::CanvasInterface : public Geometer::Interface
{
public:

	wxDECLARE_DYNAMIC_CLASS( CanvasInterface );

	CanvasInterface( void );
	virtual ~CanvasInterface( void );

	virtual void FillOutRegistryEntry( RegistryEntry& entry ) override;
	virtual bool CreateControls( void ) override;
	virtual bool UpdateControls( void ) override;
};

#endif //__CanvasInterface_h__

// CanvasInterface.h