// ConsoleInputInterface.h

#ifndef __ConsoleInputInterface_h__
#define __ConsoleInputInterface_h__

#include <wx/textctrl.h>
#include <wx/sizer.h>

#include "Interface.h"

namespace Geometer
{
	class ConsoleInputInterface;
}

//========================================================================
class Geometer::ConsoleInputInterface : public Geometer::Interface
{
public:

	wxDECLARE_DYNAMIC_CLASS( ConsoleInputInterface );

	ConsoleInputInterface( void );
	virtual ~ConsoleInputInterface( void );

	virtual void FillOutRegistryEntry( RegistryEntry& entry ) override;
	virtual bool CreateControls( void ) override;
	virtual bool UpdateControls( void ) override;

private:

	void OnExecute( wxCommandEvent& event );

	wxTextCtrl* textControl;
};

#endif //__ConsoleInputInterface_h__

// ConsoleInputInterface.h