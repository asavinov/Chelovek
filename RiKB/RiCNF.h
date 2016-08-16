//================================================================
// RiCNF  
//================================================================
// This node is a conjunctive normal form represeniting intensionally 
// some distribution over the multi-dimenisonal space
class RiCNF : public RiNode {
public:

  RiSpace* Space;

  RiOptInduction* Options;

  RiSVHeader* Matrix;
  RiSVHeader* NewMatrix;

  RiLine Lines; // The number of lines (sectioned vectors) in the matrix
  RiLine FreeLines; // The number of empty lines (sectioned vectors) in the matrix (not used)

  RiComp4 Min;
  RiComp4 Max;
  RiComp4 Freq;

  RiCNF( RiSpace* NewSpace, RiOptInduction* NewOptions, char* NewName );
  ~RiCNF();

  virtual RiError Init() = 0; // Initialize semantics (add one trivial line)
  virtual RiError Empty() = 0; // Empty semantics (remove all lines)
	};



//================================================================
// RiCNF_B
//================================================================
// Crisp (boolean) CNF
class RiCNF_B : public RiCNF {
public:

  RiCNF_B( RiSpace* NewSpace, RiOptInduction* NewOptions, char* NewName );
  ~RiCNF_B();

  RiError StartInduction();
  RiError EndInduction();
  RiError AddPoint( RiCoord* Point );

  RiLine AbsorbByPrev( RiSVHeader* ptr1, RiSVHeader* ptr2 );
  RiLine AbsorbPrev( RiSVHeader* ptr1, RiSVHeader* ptr2 );

  RiLine ReorderMatrix( RiSVHeader* ptr1, RiLine LinesNum );

  virtual RiError Init();
  virtual RiError Empty();

  int TextSV( RiLine LineNo, char* Text );
  int Text( RiLine LineNo, char* Text );
  int Text( RiLine LineNo, RiCoord VarNo, char* Text );

  BOOL IsEmpty( RiLine LineNo, RiCoord VarNo );
  RiComp1 RiCNF_B::GetComp( RiLine LineNo, RiCoord VarNo, RiCoord ValNo );
  RiComp1 RiCNF_B::SetComp( RiLine LineNo, RiCoord VarNo, RiCoord ValNo, RiComp1 Comp );

	};

