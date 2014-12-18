// List.h

#pragma once

template< class Data >
class GACOMPUTE_API BlankDataManager
{
public:
	static void Create( Data& data );
	static void Destroy( Data& data );
	static void Copy( Data& dataA, const Data& dataB );
	static int SortCompare( const Data& dataA, const Data& dataB );
};

template< class Data >
class GACOMPUTE_API DefaultDataManager
{
public:
	static void Create( Data& data );
	static void Destroy( Data& data );
	static void Copy( Data& dataA, const Data& dataB );
	static int SortCompare( const Data& dataA, const Data& dataB );
};

template< class Data, class DataManager = DefaultDataManager< Data > >
class GACOMPUTE_API List
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

		const Node* Next( void ) const;
		const Node* Prev( void ) const;

	private:

		void Couple( Node* before, Node* after );
		void Decouple( void );

		Node* next;
		Node* prev;
	};

	int Count( void ) const;

	Node* Head( void );
	Node* Tail( void );

	const Node* Head( void ) const;
	const Node* Tail( void ) const;

	Node* InsertBefore( Node* node = 0, Node* insertedNode = 0 );
	Node* InsertAfter( Node* node = 0, Node* insertedNode = 0 );

	bool Remove( Node* node, bool deleteNode = true );
	bool RemoveAll( void );

	void Absorb( List* list );
	void Concatinate( const List& list );
	void Copy( const List& list );

	enum SortOrder { SORT_ASCENDING, SORT_DESCENDING };
	int Sort( SortOrder sortOrder );

private:

	Node* head;
	Node* tail;
	int count;
};

#include "List.hpp"

// List.h