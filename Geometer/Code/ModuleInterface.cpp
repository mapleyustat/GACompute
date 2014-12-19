// ModuleInterface.cpp

#include "ModuleInterface.h"
#include "Application.h"

namespace Geometer
{
	wxIMPLEMENT_DYNAMIC_CLASS( ModuleInterface, Interface )
}

using namespace Geometer;

ModuleInterface::ModuleInterface( void )
{
}

/*virtual*/ ModuleInterface::~ModuleInterface( void )
{
}

/*virtual*/ void ModuleInterface::FillOutRegistryEntry( RegistryEntry& entry )
{
	entry.interfaceMenuItemName = "Modules";
	entry.interfaceMenuItemHelp = "Manage Lua modules.";
}

/*virtual*/ bool ModuleInterface::CreateControls( void )
{
	dataViewCtrl = new wxDataViewCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE | wxDV_ROW_LINES );

	wxObjectDataPtr< DataViewModel > dataViewModel;
	dataViewModel = new DataViewModel();
	dataViewCtrl->AssociateModel( dataViewModel.get() );

	dataViewCtrl->AppendTextColumn( "Entry", 0, wxDATAVIEW_CELL_INERT );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( dataViewCtrl, 1, wxGROW );
	SetSizer( boxSizer );

	return true;
}

/*virtual*/ bool ModuleInterface::UpdateControls( void )
{
	// STPTODO: Tell the data view control to update here.

	return true;
}

ModuleInterface::DataViewModel::DataViewModel( void )
{
	nextId = 1;
}

/*virtual*/ ModuleInterface::DataViewModel::~DataViewModel( void )
{
}

/*virtual*/ unsigned int ModuleInterface::DataViewModel::GetColumnCount() const
{
	return 1;
}

/*virtual*/ wxString ModuleInterface::DataViewModel::GetColumnType( unsigned int col ) const
{
	return "";
}

/*virtual*/ void ModuleInterface::DataViewModel::GetValue( wxVariant& variant, const wxDataViewItem& item, unsigned int col ) const
{
	int id = ( int )item.m_pItem;

	ItemMap::const_iterator iter = itemMap.find( id );
	if( iter == itemMap.end() )
		return;

	const Entry& entry = iter->second;

	if( col == 0 )
		variant = entry.label;
	else
	{
		//...
	}
}

/*virtual*/ bool ModuleInterface::DataViewModel::SetValue( const wxVariant& variant, const wxDataViewItem& item, unsigned int col )
{
	return false;
}

/*virtual*/ wxDataViewItem ModuleInterface::DataViewModel::GetParent( const wxDataViewItem& item ) const
{
	wxDataViewItem invalidItem;
	return invalidItem;
}

/*virtual*/ bool ModuleInterface::DataViewModel::IsContainer( const wxDataViewItem& item ) const
{
	if( !item.IsOk() )
		return true;

	return false;
}

/*virtual*/ unsigned int ModuleInterface::DataViewModel::GetChildren( const wxDataViewItem& item, wxDataViewItemArray& children ) const
{
	if( !item.IsOk() )
	{
		nextId = 1;
		itemMap.clear();

		lua_State* L = wxGetApp().LuaState();

		int stackTop = lua_gettop( L );

		lua_getglobal( L, "package" );
		if( lua_isnil( L, -1 ) )
			return 0;

		lua_getfield( L, -1, "loaded" );

		lua_pushnil( L );
		while( lua_next( L, -2 ) )
		{
			const char* moduleName = lua_tostring( L, -2 );

			if( 0 != strcmp( moduleName, "_G" ) )
			{
				Entry entry;
				entry.module = moduleName;
				entry.label = moduleName;
				entry.description = "";
				entry.type = Entry::MODULE;

				int id = nextId++;
				itemMap[ id ] = entry;

				wxDataViewItem item;
				item.m_pItem = ( void* )id;
				children.push_back( item );
			}

			lua_pop( L, 1 );
		}

		while( stackTop < lua_gettop( L ) )
			lua_pop( L, 1 );
	}
	else
	{
		// STPTODO: Lookup the entry's module and iterate over api calls here.
	}

	return children.Count();
}

// ModuleInterface.cpp