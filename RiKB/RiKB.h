//================================================================
// RiKB  
//================================================================
// This class represents knowledge base consisting of syntax 
// and semantics (a set of rules)
class RiKB : public RiNode {
public:

  // Main objects of the knowledge base
  RiSpace* Space; // Only one space (syntax)
  RiCNF_B* Rules; // Only one knowledge base module (semantics)
  RiData_Dao* DataSource; // Only one data source

  RiProcess Process;

  RiOptAnalysis OptAnalysis; // Analysis process options
  RiOptInduction OptInduction; // Rule induction process options

  // General functions
  RiKB( char* NewName );
  ~RiKB();
  RiError Save();
  RiError Load();

  // Process functions
  RiError ProcessBegin( RiProcess NewProcess );
  RiError ProcessEnd();
  RiError ProcessPoint( RiCoord* Point, RiComp Component );
  RiError FindPatternsBegin();
  RiError FindPatternsEnd();

  // Syntax functions
  RiCoord VarNumber();
  RiError VarAdd( char* VarName );
  RiError VarDelete( RiCoord VarNo );
  RiError VarDelete( char* VarName );
  int VarGetName( RiCoord VarNo, char* VarName );
  RiCoord VarGetNo( char* VarName );
  // VarSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 
  // VarCluster();

  RiCoord ValNumber( RiCoord VarNo );
  RiCoord ValNumber( char* VarName );
  RiError ValAdd( RiCoord VarNo, char* ValName );
  RiError ValAdd( char* VarName, char* ValName );
  RiError ValDelete( RiCoord VarNo, RiCoord ValNo );
  RiError ValDelete( char* VarName, char* ValName );
  int ValGetName( RiCoord VarNo, RiCoord ValNo, char* ValName );
  RiCoord ValGetNo( char* VarName, char* ValName );
  // ValSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 

  // Semantics functios (knowledge base properties) 

  // RuleFunctions
  RuleNumber();
  RuleAdd();
  RuleDelete();
  RuleGet(); // Get general properties (Min, Max, etc.)
  // RuleSort(); // Null, Rank, Info 
  // RuleCluster();
  // RuleFilter(); // Pos, Neg, Rank, VarInvolved

  RuleCompGet();
  RuleCompSet();

	};


