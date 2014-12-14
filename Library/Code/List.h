// List.h

#pragma once

template< typename Data >
class BlankDataManager
{
public:
	static void Create( Data& data );
	static void Destroy( Data& data );
	static int SortCompare( const Data& dataA, const Data& dataB );
};

template< typename Data >
class DefaultDataManager
{
public:
	static void Create( Data& data );
	static void Destroy( Data& data );
	static int SortCompare( const Data& dataA, const Data& dataB );
};

template< typename Data, class DataManager = DefaultDataManager< Data > >
class List
{
public:
	
	List( void );
	~List( void );

	class Node
	{
		friend class List;

	public:

		Node( void );
		~Node( void );

		Data data;

		Node* Next( void );
		Node* Prev( void );

	private:

		void Couple( Node* before, Node* after );
		void Decouple( void );

		Node* next;
		Node* prev;

		List* list;
	};

	int Count( void ) const;

	Node* Head( void );
	Node* Tail( void );

	Node* InsertBefore( Node* node = 0, Node* insertedNode = 0 );
	Node* InsertAfter( Node* node = 0, Node* insertedNode = 0 );

	bool Remove( Node* node, bool deleteNode = true );
	bool RemoveAll( void );

	void Absorb( List* list );

	typedef int ( *SortCompareFunc )( Data dataA, Data dataB );

	enum SortOrder { SORT_ASCENDING, SORT_DESCENDING };
	int Sort( SortOrder sortOrder, SortCompareFunc sortCompareFunc );

private:

	Node* head;
	Node* tail;
	int count;
};

#include "List.hpp"

// List.h