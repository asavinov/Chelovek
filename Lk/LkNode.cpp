#include "stdafx.h"
#include "Lk.h"

//
// Construction
//

LkNode::LkNode()
{
	m_Name = NULL;

	m_NumChildren = 0;
	m_NumParents = 0;

	m_Children = NULL;
	m_Parents = NULL;
}

LkNode::LkNode( char* Name )
{
	// Allocate memory for the name string
	if ( Name != NULL )
	{
		m_Name = (char*) GlobalAlloc( GPTR, lstrlen(Name) + 1 );
		if ( m_Name == NULL ) 
			; // ERROR: LkError_Memory

		lstrcpy( m_Name, Name ); // and copy the string
	}
  else
	{
		m_Name = NULL;
	}

	m_NumChildren = 0; // no children
	m_NumParents = 0; // no parents

	m_Children = NULL;
	m_Parents = NULL;
}

//
// Distruction
//

LkNode::~LkNode()
{
	// Delete all children
	while ( m_NumChildren )
	{
		delete m_Children[m_NumChildren-1];
	}
	// Now we do not have any children, i.e., we are the terminating node

	// detach us from each of our parents
	while ( m_NumParents )
	{
		m_Parents[m_NumParents-1]->Exclude( this );
	}

	// and then we can free our memory since we are not linked to anybody
	if ( m_Children != NULL ) 
		GlobalFree( m_Children );

	if ( m_Parents != NULL ) 
		GlobalFree( m_Parents );

	if ( m_Name != NULL ) 
		GlobalFree( m_Name );
}

LkCoord LkNode::Exclude( LkNode* Node )
{
	LkCoord x, y, NumExcluded = 0;

	for ( x = m_NumChildren; x; x-- )
	{
		if ( m_Children[x-1] != Node )
			continue;

		// remove it from our list of children
		MoveMemory( m_Children + x - 1, m_Children + x, (m_NumChildren - x)*sizeof(LkNode*) );
		m_NumChildren--;

		NumExcluded++;

		// remove us from its list of parents
		for ( y = Node->m_NumParents; y; y-- )
		{
			if ( Node->m_Parents[y-1] != this )
				continue;

			MoveMemory( Node->m_Parents + y - 1, Node->m_Parents + y, (Node->m_NumParents - y)*sizeof(LkNode*) );
			Node->m_NumParents--;
		}
	}

	return NumExcluded;
}

LkCoord LkNode::Exclude( LkCoord Coord )
{
	if ( (Coord >= m_NumChildren) || (Coord < 0) )
		return 0; // ERROR: Wrong coordinate

	LkNode* Node = m_Children[Coord];

	// remove it from our list of children
	MoveMemory( m_Children + Coord, m_Children + Coord + 1, (m_NumChildren - Coord - 1)*sizeof(LkNode*) );
	m_NumChildren--;

	// remove us from its list of parents
	for ( LkCoord y = Node->m_NumParents; y; y-- )
	{
		if ( Node->m_Parents[y-1] != this )
			continue;

		MoveMemory( Node->m_Parents + y - 1, Node->m_Parents + y, (Node->m_NumParents - y)*sizeof(LkNode*) );
		Node->m_NumParents--;
	}

	return 1;
}

LkCoord LkNode::Exclude()
{
	LkCoord NumExcluded = m_NumChildren;

	while ( m_NumChildren )
	{
		Exclude( m_NumChildren - 1 );
	}

	return m_NumChildren;
}

LkError LkNode::Include( LkNode* Node )
{
	LkNode** NewMemory;
	// Reallocate additional memory if necessary (we need ArrayLength+1 pointers)
	if ( m_Children == NULL )
	{
		NewMemory = (LkNode**) GlobalAlloc( GPTR, (m_NumChildren + 1)*sizeof(LkNode*) );
		if (NewMemory == NULL)
			return LkError_Memory; // ERROR: Low memory

		m_Children = NewMemory;
	}
	else if ( GlobalSize(m_Children) < ((m_NumChildren + 1)*sizeof(LkNode*)) )
	{
		NewMemory = (LkNode**) GlobalReAlloc( (HGLOBAL) m_Children, (m_NumChildren + 1)*sizeof(LkNode*), GMEM_MOVEABLE );
		if (NewMemory == NULL)
			return LkError_Memory; // ERROR: Low memory

		m_Children = NewMemory;
	}

	if ( Node->m_Parents == NULL )
	{
		NewMemory = (LkNode**) GlobalAlloc( GPTR, (Node->m_NumParents + 1)*sizeof(LkNode*) );
		if (NewMemory == NULL)
			return LkError_Memory; // ERROR: Low memory

		Node->m_Parents = NewMemory;
	}
	else if ( GlobalSize(Node->m_Parents) < ((Node->m_NumParents + 1)*sizeof(LkNode*)) )
	{
		NewMemory = (LkNode**) GlobalReAlloc( (HGLOBAL) Node->m_Parents, (Node->m_NumParents + 1)*sizeof(LkNode*), GMEM_MOVEABLE );
		if (NewMemory == NULL)
			return LkError_Memory; // ERROR: Low memory

		Node->m_Parents = NewMemory;
	}

	LkCoord x = m_NumChildren; // Set right coordinate (index) for the new node

	// Add node to the list of children
	MoveMemory( m_Children + x + 1, m_Children + x, (m_NumChildren - x)*sizeof(LkNode*) );
	m_Children[x] = Node;
	m_NumChildren++;

	// Now link the new object to us, i.e., append us to its list of parents
	Node->m_Parents[Node->m_NumParents] = this;
	Node->m_NumParents++;

	return LkError_Success;
}

int LkNode::GetName( char* NameBuffer )
{
	if ( m_Name == NULL ) return -1;

	int Length = lstrlen( m_Name );

	if ( NameBuffer == NULL )
		return Length;

	lstrcpy( NameBuffer, m_Name );

	return Length;
}

int LkNode::SetName( char* Name )
{
	int Length;

	if ( Name == NULL )
	{
		if ( m_Name != NULL )
		{
			Length = lstrlen( m_Name );
			GlobalFree( m_Name );
			m_Name = NULL;
		}
		else
			Length = -1;

		return Length;
	}

	Length = lstrlen( Name );

  // Check if there is a room for new name
	if ( m_Name != NULL )
		if ( GlobalSize(m_Name) < (Length + 1) )
		{
			char* NewMemory = (char*) GlobalReAlloc( (HGLOBAL) m_Name, Length + 1, GMEM_MOVEABLE );
			if (NewMemory == NULL)
				return LkError_Memory; // ERROR:

			m_Name = NewMemory;
		}
	else
	{
		char* NewMemory = (char*) GlobalAlloc( GPTR, Length + 1 );
		if (NewMemory == NULL)
			return LkError_Memory; // ERROR:

		m_Name = NewMemory;
	}

	lstrcpy( m_Name, Name );

	return Length;
}

LkCoord LkNode::Object2Coord( LkNode* ChildNode )
{
	// Scan through the list of children and find given pointer
	for ( LkCoord x = 0; x < m_NumChildren; x++ )
		if ( m_Children[x] == ChildNode )
			break;

	return x;
}

LkNode* LkNode::Coord2Object( LkCoord Coord )
{
#ifdef _DEBUG
	if ( Coord >= m_NumChildren || Coord < 0 )
		return NULL; // ERROR: Wrong coordinate
#endif // _DEBUG

	// Return the pointer from the list of children with the given index
	return m_Children[Coord];
}

LkNode* LkNode::Name2Object( char* ObjName )
{
	// Return the first pointer from the list of children with the given name

	// Scan through the list of children and find given name
	for ( LkCoord x = 0; x < m_NumChildren; x++ )
		if ( ObjName == NULL )
		{
			if ( m_Children[x]->m_Name == NULL )
				break;
		}
		else
		{
			if ( lstrcmpi(m_Children[x]->m_Name, ObjName) == 0 )
				break;
		}

	if ( x < m_NumChildren )
		return m_Children[x];
	else
		return NULL;
}

LkCoord LkNode::Name2Coord( char* ObjName )
{
	// Return the first object coordinate from the list of children with the given name

	// Scan through the list of children and find given name
	for ( LkCoord x = 0; x < m_NumChildren; x++ )
		if ( ObjName == NULL )
		{
			if ( m_Children[x]->m_Name == NULL )
				break;
		}
		else
		{
			if ( lstrcmpi(m_Children[x]->m_Name, ObjName) == 0 )
				break;
		}

	return x;
}

LkCoord LkNode::N()
{
	return m_NumChildren;
}
