// List.hpp

template< typename Data >
inline /*static*/ void DefaultDataFactory< Data >::Create( Data& data )
{
}

template< typename Data >
inline /*static*/ void DefaultDataFactory< Data >::Destroy( Data& data )
{
}

template< typename Data >
inline /*static*/ void DeletingDataFactory< Data >::Create( Data& data )
{
	data = 0;
}

template< typename Data >
inline /*static*/ void DeletingDataFactory< Data >::Destroy( Data& data )
{
	delete data;
	data = 0;
};

template< typename Data, class DataFactory >
inline List< Data, DataFactory >::List( void )
{
	count = 0;

	head = 0;
	tail = 0;
}

template< typename Data, class DataFactory >
inline List< Data, DataFactory >::~List( void )
{
	RemoveAll();
}

template< typename Data, class DataFactory >
inline List< Data, DataFactory >::Node::Node( void )
{
	next = 0;
	prev = 0;

	list = 0;

	DataFactory::Create( data );
}

template< typename Data, class DataFactory >
inline List< Data, DataFactory >::Node::~Node( void )
{
	DataFactory::Destroy( data );
}

template< typename Data, class DataFactory >
inline void List< Data, DataFactory >::Node::Couple( Node* before, Node* after )
{
	before->next = this;
	after->prev = this;

	next = after;
	prev = before;
}

template< typename Data, class DataFactory >
inline void List< Data, DataFactory >::Node::Decouple( void )
{
	if( next )
		next->prev = prev;
	if( prev )
		prev->next = next;

	next = 0;
	prev = 0;
}

template< typename Data, class DataFactory >
inline int List< Data, DataFactory >::Count( void ) const
{
	return count;
}

template< typename Data, class DataFactory >
inline typename List< Data, DataFactory >::Node* List< Data, DataFactory >::Head( void )
{
	return head;
}

template< typename Data, class DataFactory >
inline typename List< Data, DataFactory >::Node* List< Data, DataFactory >::Tail( void )
{
	return tail;
}

template< typename Data, class DataFactory >
inline typename List< Data, DataFactory >::Node* List< Data, DataFactory >::InsertBefore( Node* node /*= 0*/, Node* insertedNode /*= 0*/ )
{
	if( !insertedNode )
		insertedNode = new Node();

	if( count == 0 )
	{
		head = insertedNode;
		tail = insertedNode;
	}
	else
	{
		if( !node )
			node = head;

		insertedNode->Couple( node, node->next );
	}

	count++;
	insertedNode->list = this;

	return insertedNode;
}

template< typename Data, class DataFactory >
inline typename List< Data, DataFactory >::Node* List< Data, DataFactory >::InsertAfter( Node* node /*= 0*/, Node* insertedNode /*= 0*/ )
{
	if( !insertedNode )
		insertedNode = new Node();

	if( count == 0 )
	{
		head = insertedNode;
		tail = insertedNode;
	}
	else
	{
		if( !node )
			node = tail;

		insertedNode->Couple( node->prev, node );
	}

	count++;
	insertedNode->list = this;

	return insertedNode;
}

template< typename Data, class DataFactory >
inline bool List< Data, DataFactory >::Remove( Node* node, bool deleteNode /*= true*/ )
{
	if( node->list != this )
		return false;

	if( node == head )
		head = head->next;
	if( node == tail )
		tail = tail->prev;

	node->Decouple();

	if( deleteNode )
		delete node;

	count--;

	return true;
}

template< typename Data, class DataFactory >
inline bool List< Data, DataFactory >::RemoveAll( void )
{
	while( count > 0 )
		if( !Remove( head, true ) )
			return false;

	return true;
}

template< typename Data, class DataFactory >
inline void List< Data, DataFactory >::Absorb( List* list )
{
	if( count == 0 )
	{
		head = list->head;
		tail = list->tail;
	}
	else
	{
		tail->next = list->head;
		tail->next->prev = tail;
		tail = list->tail;
	}

	count = list->count;

	list->head = 0;
	list->tail = 0;
	list->count = 0;
}

template< typename Data, class DataFactory >
inline int List< Data, DataFactory >::Sort( SortOrder sortOrder, SortCompareFunc sortCompareFunc )
{
	int adjacentSwapCount = 0;

	bool keepGoing = true;
	for( keepGoing )
	{
		keepGoing = false;

		Node* node = head;
		while( node && node->next )
		{
			Node* next = node->next;

			int result = sortCompareFunc( node->data, next->data );

			if( ( result > 0 && sortOrder == SORT_ASCENDING ) || ( result < 0 && sortOrder == SORT_DESCENDING ) )
			{
				Remove( node, false );
				InsertAfter( next, node );

				keepCount = true;
				adjacentSwapCount++;
			}
			else
			{
				node = next;
			}
		}
	}

	return adjacentSwapCount;
}

// List.hpp