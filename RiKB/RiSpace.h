//================================================================
// RiSpace
//================================================================
// This class represents the syntax of the knowledge base, i.e., 
// some multi-dimensional space with its properties 
class RiSpace : public RiNode {
public:

  RiCoord VarNumber; // The number of variables (dimensions)
  RiCoord* ValNumber; // The number of values for each variable (vector)

  RiCoord ValSum; // The number of values of all variables (sum of all values)
  RiOffs ValProduct; // The number of points in the space (Power of the universe of discourse)

  RiCoord* AccSum; // Vector of accumulated sum of values
  RiOffs* AccProduct; // Vector of accumulated product of values

	RiSpace( char* NewName );
  ~RiSpace();

  // Variable functions
  RiError VarAdd( char* VarName );
  RiError VarDelete( RiCoord VarNo );
  RiError VarDelete( char* VarName );
  int VarGetName( RiCoord VarNo, char* VarName );
  RiCoord VarGetNo( char* VarName );
  // VarSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 
  // VarCluster();

  RiError ValAdd( RiCoord VarNo, char* ValName );
  RiError ValAdd( char* VarName, char* ValName );
  RiError ValDelete( RiCoord VarNo, RiCoord ValNo );
  RiError ValDelete( char* VarName, char* ValName );
  int ValGetName( RiCoord VarNo, RiCoord ValNo, char* ValName );
  RiCoord ValGetNo( char* VarName, char* ValName );
  // ValSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 

  RiCoord RiSpace::FillValNumber();
  RiCoord RiSpace::FillAccSum();
  RiOffs RiSpace::FillAccProduct();

  RiCoord RiSpace::CoordSum();
  RiOffs RiSpace::CoordProduct();

  RiError RecalcParams();

  };



//================================================================
// RiVariable
//================================================================
// Special node which describes one variable (space dimension).
class RiVariable : public RiNode {
public:

  RiVarType Type; // Data type of the column (test string, integer etc.)
  RiComp4 Freq;

	RiVariable( char* NewName, RiVarType NewType );
  ~RiVariable();

  // Parameters
  RiVarType GetType();
  RiVarType SetType( RiVarType NewType );

  };



//================================================================
// RiValue
//================================================================
// Special node which describes one value of variable
class RiValue : public RiNode {
public:

  RiValType Type;
  RiComp4 Freq;

	RiValue( char* NewName, RiValType NewType );
  ~RiValue();

  // Parameters
  RiValType GetType();
  RiValType SetType( RiValType NewType );

  };



