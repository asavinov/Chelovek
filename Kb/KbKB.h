#if !defined(__KbKB_h__)
#define __KbKB_h__

/*
This class represents knowledge base consisting syntax, 
semantics (a set of rules), data source and other objects.
That is it is simply a set of objects (including options).
*/
class KbKB : public LkNode
{
public:

	// Main objects of the knowledge base
	KbDataSourceCSV *DataSource; // Only one data source

	RiSpace* Space; // Only one space (syntax)
	RiCNF_B* Rules; // Only one knowledge base module (semantics)

	RiOptAnalysis OptAnalysis; // Analysis process options
	RiOptInduction OptInduction; // Rule induction process options

public:

	// Construction/destruction
	KbKB( char* Name );
	virtual ~KbKB();

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

#endif // !defined(__KbKB_h__)
