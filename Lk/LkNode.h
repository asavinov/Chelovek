#if !defined(__LkNode_h__)
#define __LkNode_h__

/*
This is general class which should be used as a base class for 
various concrete applied classes. Its main property is that 
each object based on it has a set of children a set of parents.
These set are finite and can be accessed by their coordinate, 
i.e. position in the list. In addition, each node is 
characterized by a name.
*/
class LkNode
{
private:

	char* m_Name; // Any node has a name

	LkCoord m_NumChildren;
	LkCoord m_NumParents;

	LkNode** m_Children; // Pointer to the array of pointers to child nodes
	LkNode** m_Parents; // Pointer to the array of pointers to parent nodes

public:

	LkNode();
	LkNode( char* Name );

	virtual ~LkNode();

	// Exclude/Include children functions
	LkCoord Exclude( LkNode* Node ); // Exclude the pointer (all) from the list of children
	LkCoord Exclude( LkCoord Coord ); // Exclude the coordinate from the list of children
	LkCoord Exclude(); // Exclude all children

	LkError Include( LkNode* Node ); // Include the pointer to the end of the list of children

	// Parameters
	int GetName( char* NameBuffer ); // Copy name to buffer. Return the string length
	int SetName( char* NewName ); // Set new name. Return the length of the new name

	// Transformations between coordinates and objects
	LkCoord Object2Coord( LkNode* ChildNode );
	LkNode* Coord2Object( LkCoord Coord );
	LkNode* Name2Object( char* ObjName );
	LkCoord Name2Coord( char* ObjName );

	LkCoord N();
  };

#endif // !defined(__LkNode_h__)
