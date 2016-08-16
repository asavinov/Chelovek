#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiNode                                   >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiNode::RiNode( char* NewName, RiOperation NewOperation ) {
//----------------------------------------------------------------

  // Allocate memory for the name string
  if ( NewName != NULL ) {
    Name = (char*) GlobalAlloc( GPTR, lstrlen(NewName) + 1 );
    if ( Name == NULL ) 
      ; // ERROR: RiError_Memory

    // and copy name their
    lstrcpy( Name, NewName );

    }
  else {
    Name = NULL; // Allow NULL names for nodes
    }

  Operation = NewOperation;

  ChildrenNumber = 0; // no children
  ParentsNumber = 0; // no parents

  // Allocate memory for two (empty) arrays of pointers

  Children = (RiNode**) GlobalAlloc( GPTR, sizeof(RiNode*) );
  if ( Children == NULL ) ; // ERROR:

  Parents = (RiNode**) GlobalAlloc( GPTR, sizeof(RiNode*) );
  if ( Parents == NULL ) ; // ERROR:

  // and initialize them (each array of pointers is ended by a NULL pointer)
  Children[0] = NULL;
  Parents[0] = NULL;

  }

//================================================================
// destructor. It deletes the node as a directory, i.e., 
// with all children (the whole tree)
RiNode::~RiNode() {
//----------------------------------------------------------------

  //
  // Delete all children
  //

  // main children
  while ( ChildrenNumber != 0 ) {
    delete Children[0];
    }

  while ( Children[0] != NULL ) {
    delete Children[0];
    }
/*
  for ( RiCoord x = 0; x < ChildrenNumber; x++ ) {
    delete Children[x];
    }

  // symbolic links
  for ( ; Children[x] != NULL; x++ ) {
    delete Children[x];
    }
*/
  // Now we do not have any children, i.e., we are the terminating node

  //
  // Thus we have to unlink ourselves from all parents
  //

  // detach us from each of our main parents
  while ( ParentsNumber != 0 ) {
    Parents[0]->Exclude( this );
    }

  // detach us from each of our symbolic parents
  while ( Parents[0] != NULL ) {
    Parents[0]->Exclude( this );
    }

  // and then we can free our memory since we are not linked to anybody
  if ( Children != NULL ) GlobalFree( Children );

  if ( Parents != NULL ) GlobalFree( Parents );

  if ( Name != NULL ) GlobalFree( Name );

  }

//================================================================
// Exclude given pointer from the list of children
RiError RiNode::Exclude( RiNode* Node ) {
//----------------------------------------------------------------

  //
  // Detach the given node as a child from us
  //

  RiCoord ArrayLength = PointerArrayLength(Children);

  // find its index
  RiCoord x = Pointer2Index( Children, Node );

  if ( x == ArrayLength ) ; // ERROR: Pointer not found (nothing to exclude)

  // remove it
  MoveMemory( Children + x, Children + x + 1, (ArrayLength - x)*sizeof(RiNode*) );

  // If it is not a symbolic link
  if ( x < ChildrenNumber ) {
    ChildrenNumber--;
    }

  //
  // Now update parent information for the child in the same way, 
  // i.e., detach us as a parent from the given node
  //
  ArrayLength = PointerArrayLength(Node->Parents);

  // find index of this object
  x = Pointer2Index( Node->Parents, this );

  if ( x == ArrayLength ) ; // ERROR: Pointer not found

  MoveMemory( Node->Parents + x, Node->Parents + x + 1, (ArrayLength - x)*sizeof(RiNode*) );

  // If it is not a symbolic link
  if ( x < Node->ParentsNumber ) {
    Node->ParentsNumber--;
    }

  // Now the object Node is not included in the list of 
  // our children and does not point to us as a parent
  return RiError_Success;
  }

//================================================================
// Exclude given coordinate from the list of children
RiError RiNode::Exclude( RiCoord Coordinate ) {
//----------------------------------------------------------------

  if ( (Coordinate >= PointerArrayLength(Children)) || 
	   (Coordinate >= RiCoord_Max) )
    return RiError_General; // ERROR: Wrong coordinate

  return Exclude( Children[Coordinate] );
  }

//================================================================
// Exclude all children
RiError RiNode::Exclude() {
//----------------------------------------------------------------

  RiError Err;

  // main children
  for ( ; ChildrenNumber > 0; ) {
    Err = Exclude( ChildrenNumber - 1 );
    if ( Err != RiError_Success ) return Err;
    }

  // symbolic links
  for ( ; Children[0] != NULL; ) {
    Err = Exclude( Children[0] );
	if ( Err != RiError_Success ) return Err;
    }

  // Now we do not have any children
  // But still somebody can reference us a parent!!!
    
  return RiError_Success;
  }

//================================================================
// Include given pointer to the end of the list of children
RiError RiNode::Include( RiNode* Node ) {
//----------------------------------------------------------------

  RiCoord ArrayLength = PointerArrayLength(Children);
  RiCoord x;

  //
  // Check if there exists a child with such name in a loop on all children
  // NULL names are considered unique
  //
  if ( Node->Name != NULL ) {

    for ( x = 0; x < ArrayLength; x++ ) {
      if ( NULL == lstrcmpi( Children[x]->Name, Node->Name) ) break;
      }
    if ( x != ArrayLength ) 
      return RiError_General; // ERROR: Repeated non-NULL name

    }

  // The name of the new node is ok so try to attach it

  // Set right coordinate (index) for the new node
  x = ArrayLength;

  // Reallocate additional memory if necessary (we need ArrayLength+2 pointers)
  RiNode** NewMemory;
  if ( GlobalSize(Children) < ((ArrayLength + 2)*sizeof(RiNode*)) ) {
    NewMemory = (RiNode**) GlobalReAlloc( (HGLOBAL) Children, (ArrayLength + 2)*sizeof(RiNode*), GMEM_MOVEABLE );
    if (NewMemory == NULL)
      return RiError_General; // ERROR:
    else
      Children = NewMemory;
    }

  if ( GlobalSize(Node->Parents) < ((PointerArrayLength(Node->Parents) + 2)*sizeof(RiNode*)) ) {
    NewMemory = (RiNode**) GlobalReAlloc( (HGLOBAL) Node->Parents, (PointerArrayLength(Node->Parents) + 2)*sizeof(RiNode*), GMEM_MOVEABLE );
    if (Node->Parents == NULL)
      return RiError_General; // ERROR:
    else
      Node->Parents = NewMemory;
    }

  //
  // Add node to the list of children
  //

  // make room for one new pointer in the array
  MoveMemory( Children + x + 1, Children + x, (ArrayLength - x + 1)*sizeof(RiNode*) );

  // and write the pointer
  Children[x] = Node;

  // if not symbolic link
  if ( x <= ChildrenNumber ) {
    ChildrenNumber++;
    }

  //
  // Now link the new object to us. i.e., append us to its list of parents
  //

  ArrayLength = PointerArrayLength(Node->Parents);

  // we do not need to make a room since we append new pointer to the end of list of parents

  // and write the pointer
  Node->Parents[ArrayLength] = this;
  Node->Parents[ArrayLength+1] = NULL;

  // if not symbolic link
  if ( Node->ParentsNumber == ArrayLength ) {
    Node->ParentsNumber++;
    }

  // Now the object Node is included in the list of children and points to us as a parent
  return RiError_Success;
  }

//================================================================
// Copy name to buffer. It returns the length of the string
int RiNode::GetName( char* NameBuffer ) {
//----------------------------------------------------------------

  if ( Name == NULL ) return -1;

  int Length = lstrlen( Name );

  if ( NameBuffer == NULL ) return Length;

  lstrcpy( NameBuffer, Name );

  return Length;
  }

//================================================================
// Set new name. It returns the length of the new name
int RiNode::SetName( char* NewName ) {
//----------------------------------------------------------------

  unsigned int Length;

  if ( NewName == NULL ) {

    if ( Name != NULL ) {
      Length = lstrlen( Name );
      GlobalFree( Name );
      }

    Name = NULL;

    return Length;
    }

  Length = lstrlen( NewName );


  //
  // Check for all parents if there exists a child with such name
  // NULL names are considered unique, i.e., they differ from all other names
  //

  // todo!!!

  //
  // Check if there is a room for new name
  //
  if ( GlobalSize(Name) < (Length + 1) ) {
    char* NewMemory = (char*) GlobalReAlloc( (HGLOBAL) Name, Length + 1, GMEM_MOVEABLE );
    if (NewMemory == NULL)
      return RiError_Memory; // ERROR:
    else
      Name = NewMemory;
    }

  lstrcpy( Name, NewName );

  return Length;
  }

//================================================================
// Get operation
RiOperation RiNode::GetOperation() {
//----------------------------------------------------------------
  return Operation;
  }

//================================================================
// Set operation.  It returns the old opearation
RiOperation RiNode::SetOperation( RiOperation NewOperation ) {
//----------------------------------------------------------------

  RiOperation ret = Operation;

  Operation = NewOperation;

  return ret;
  }

//================================================================
//
RiCoord RiNode::Object2Coord( RiNode* ChildNode ) {
//----------------------------------------------------------------

  // In the list of all children find given pointer and return its index

  // Scan through the list of children and find given pointer
  for ( RiCoord x = 0; (Children[x] != ChildNode) && (Children[x] != NULL); x++ ) ;

  return x;
  }

//================================================================
//
RiNode* RiNode::Coord2Object( RiCoord Coordinate ) {
//----------------------------------------------------------------

  // Return the pointer from the list of children with the given index

  if ( (Coordinate >= PointerArrayLength(Children)) || 
	   (Coordinate >= RiCoord_Max) )
    return NULL; // ERROR: Wrong coordinate

  return Children[Coordinate];
  }

//================================================================
//
RiNode* RiNode::Name2Object( char* ObjName ) {
//----------------------------------------------------------------

  // Return the pointer from the list of children with the given name

  if ( (ObjName == NULL) || (lstrlen(ObjName) == 0) )
    return NULL; // ERROR: Wrong name

  // Scan through the list of children and find given name
  for ( RiCoord x = 0; Children[x] != NULL && lstrcmpi(Children[x]->Name, ObjName) != 0; x++ ) ;

  return Children[x];
  }

//================================================================
//
RiCoord RiNode::Name2Coord( char* ObjName ) {
//----------------------------------------------------------------

  if ( ObjName == NULL )
    return RiCoord_Null; // ERROR: Wrong name

  // Scan through the list of children and find given name
  for ( RiCoord x = 0; Children[x] != NULL; x++ ) {
 
    if ( lstrcmpi( Children[x]->Name, ObjName ) == 0 )
      break;

    }

  if ( Children[x] == NULL ) x = RiCoord_Null; // Return RiCoord_Null if not found

  return x;
  }

//================================================================
// The number of children
RiCoord RiNode::n() {
//----------------------------------------------------------------

  return ChildrenNumber;

  }

