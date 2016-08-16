//================================================================
// RiRules 
//================================================================
// This class represents a set of rules, i.e., semantics of the 
// knowledge base. One rule -- two vectors.
class RiRules : public RiNode {
public:

  RiSpace* Space;

  RiSVHeader* Matrix;

  RiOffs LinesNumber; // The number of lines (sectioned vectors) in the matrix
  RiOffs FreeLinesNumber; // The number of empty lines (sectioned vectors) in the matrix (not used)

  // Aggregate parameters 
  RiComp4 Max;
  RiComp4 Min;

  RiRules( RiSpace* NewSpace, char* NewName );
  ~RiRules();
  };


