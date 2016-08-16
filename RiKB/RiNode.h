//================================================================
// RiNode
//================================================================
// The main purpose of this class object is to represent 
// information on the strucuture of the universe of 
// discourse.

class RiNode {
public:

  char* Name; // Any node has a name

  RiOperation Operation; // Operation used to combine children of the node

  RiCoord ChildrenNumber;
  RiCoord ParentsNumber;

  RiNode** Children; // Pointer to the array of pointers to child nodes
  RiNode** Parents; // Pointer to the array of pointers to parent nodes

  RiNode( char* NewName, RiOperation NewOperation );

  ~RiNode();

  // Exclude/Include children functions
  RiError Exclude( RiNode* Node );
  RiError Exclude( RiCoord Coord );
  RiError Exclude();

  RiError Include( RiNode* Node );

  // Parameters
  int GetName( char* NameBuffer );
  int SetName( char* NewName );
  RiOperation GetOperation();
  RiOperation SetOperation( RiOperation NewOperation );

  // Transformations between coordinates and objects
  RiCoord Object2Coord( RiNode* ChildNode );
  RiNode* Coord2Object( RiCoord Coordinate );
  RiNode* Name2Object( char* ObjName );
  RiCoord Name2Coord( char* ObjName );

  RiCoord n();
  };


