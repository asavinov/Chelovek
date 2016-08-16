#include "stdafx.h"
#include "Lk.h"

//
// Explicit instantiation of necessary classes
//
template class LkISTreeAssocLog<LkCompByte>;
template class LkISTreeAssocLog<LkCompWord>;
template class LkISTreeAssocLog<LkCompLong>;
//template class LkISTreeAssocLog<LkCompHuge>;
template class LkISTreeAssocLog<LkCompFloat>;
template class LkISTreeAssocLog<LkCompDouble>;

template <class LkCompT>
LkError LkISTreeAssocLogNode<LkCompT>::Delete()
{
	LkCoord i;
	LkISTreeAssocLogNode<LkCompT> **Child = (LkISTreeAssocLogNode<LkCompT>**)(m_Data + m_DataSize);

	for( i = 0; i < m_ChildSize; i++, Child++ )
	{
		if ( *Child )
			Child->Delete(); // Delete subtrees if not empty
	}

	// Delete ourselves, i.e., this node
	LkFree( this );

	//!!! Yet there is one pointer to us from the previous node (now invalid)
	//!!! Also there is a pointer to us from the previous node on the same level (now invalid)

	return LkError_Success;
}

template <class LkCompT>
void LkISTreeAssocLogNode<LkCompT>::Clear()
{
	LkCoord i;
	LkISTreeAssocLogNode<LkCompT> **Child = (LkISTreeAssocLogNode<LkCompT>**)(m_Data + m_DataSize);

	for( i = 0; i < m_ChildSize; i++, Child++ )
	{
		if ( *Child )
			Child->Clear(); // Clear subtrees if not empty
	}

	// Clear ourselves, i.e., this node
	for( i = 0; i < m_DataSize; i++ )
	{
		m_Data[i] = 0;
	}
}

template <class LkCompT>
LkError LkISTreeAssocLogNode<LkCompT>::Show( LkCoord Level )
{
	LkCoord i, k, n = 0;
	LkCompT *Data = m_Data;
	LkISTreeAssocLogNode<LkCompT> **Child = (LkISTreeAssocLogNode<LkCompT>**)(m_Data + m_DataSize);
	if ( m_ChildSize == 0 ) Child = NULL;

	for( i = m_DataStart; i < m_DataStart + m_DataSize; i++, Data++ )
	{
		// Indent and print item number (coordinate) and data
		for( k = 0; k < Level; k++ )
			printf("   ");

		printf("%ld: %ld\n", i, *Data );

		if ( Child == NULL )
			continue; // There are no more child nodes (the array end has been reached)

		// Recursively show tree if there is a child node
		if ( *Child != NULL )
		{
			if ( i < (*Child)->Item() )
				continue; // This child does not correspond to the current item i
			(*Child)->Show( Level + 1 );
		}

		// Calculate the next child
		Child = (++n < m_ChildSize) ? Child + 1 : NULL;
	}

	return LkError_Success;
}

// ItemSet is in reverse order

template <class LkCompT>
LkCompT LkISTreeAssocLogNode<LkCompT>::GetSupport( LkCoord *ItemSet, LkCoord Length )
{
	LkCoord i;
	LkISTreeAssocLogNode<LkCompT> **Child = (LkISTreeAssocLogNode<LkCompT>**)(m_Data + m_DataSize);
	LkISTreeAssocLogNode<LkCompT> *Node = this;

	while ( --Length > 0 ) // Follow path from this node in the loop of Length steps
	{
		if ( Node->m_ChildSize <= 0 ) // If there are no children
			return 0; // It is supposed that the support is minimal (less than minsupp)

		// Calculate pointer to the child array for the Node
		Child = (LkISTreeAssocLogNode<LkCompT>**)(Node->m_Data + Node->m_DataSize);

		// Calculate child vector index corresponding to the current item number
		i = ItemSet[Length] - (*Child)->Item(); 

		// and check it for validity
		if ( (i < 0) || (i >= Node->m_ChildSize) )
			return 0; // Index is out of range

		Node = Child[i]; // go to the next child according to the itemset path

		if ( Node == NULL ) // If there is not a child
			return 0;
	}

	// Calculate data vector index corresponding to the last item number in the itemset path
	i = ItemSet[Length] - Node->m_DataStart;

	// and check it for validity
	if ( (i < 0) || (i >= Node->m_DataSize) )
		return 0; // Index is out of range

	return Node->m_Data[i];
}

template <class LkCompT>
void LkISTreeAssocLogNode<LkCompT>::IncSupport( LkCoord *ItemSet, LkCoord Length )
{
	LkCoord i;
	LkISTreeAssocLogNode<LkCompT> **Child = (LkISTreeAssocLogNode<LkCompT>**)(m_Data + m_DataSize);

	while ( --Length >= 0 ) // Traverse item set
	{
		// Calculate data vector index corresponding to the current item number
		i = *ItemSet++ - m_DataStart;

		// and check it for validity
		if ( i < 0 )  // If less than first
			continue; // Ignore

		if ( i >= m_DataSize ) // If greater than last
			return; // Abort

		m_Data[i]++; // Increase itemset support

		if ( m_ChildSize <= 0 ) // If there are no children
			continue; // Continue with next item

		// Calculate child vector index corresponding to the current item number
		i += m_DataStart - (*Child)->Item();

		// and check it for validity
		if ( (i < 0) || (i >= m_ChildSize) )
			continue; // Index is out of range

		if ( Child[i] )
			Child[i]->IncSupport( ItemSet, Length ); // The same function for the item child and the itemset tail
	}
}





