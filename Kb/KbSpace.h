#if !defined(__KbSpace_h__)
#define __KbSpace_h__

/*
This class represents knowledge base syntax, i.e., 
some multi-dimensional space with its properties 
*/
class KbSpace : public KbNode
{
private:

	LkCoord* ValNumber; // The number of values for each variable (vector)

	LkCoord ValSum; // The number of values of all variables (sum of all values)
	KbOffs ValProduct; // The number of points in the space (Power of the universe of discourse)

	LkCoord* AccSum; // Vector of accumulated sum of values
	KbOffs* AccProduct; // Vector of accumulated product of values

public:

	KbSpace( char* NewName );
	~KbSpace();

	// Variable functions
	KbError VarAdd( char* VarName );
	KbError VarDelete( LkCoord VarNo );
	KbError VarDelete( char* VarName );
	int VarGetName( LkCoord VarNo, char* VarName );
	LkCoord VarGetNo( char* VarName );
	// VarSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 
	// VarCluster();

	KbError ValAdd( LkCoord VarNo, char* ValName );
	KbError ValAdd( char* VarName, char* ValName );
	KbError ValDelete( LkCoord VarNo, LkCoord ValNo );
	KbError ValDelete( char* VarName, char* ValName );
	int ValGetName( LkCoord VarNo, LkCoord ValNo, char* ValName );
	LkCoord ValGetNo( char* VarName, char* ValName );
	// ValSort(); // Null, Name, ValNumber, RulesNum, InfoPrice, InfoImportance..., 

	LkCoord KbSpace::FillValNumber();
	LkCoord KbSpace::FillAccSum();
	KbOffs KbSpace::FillAccProduct();

	LkCoord KbSpace::CoordSum();
	KbOffs KbSpace::CoordProduct();

	KbError RecalcParams();
};

/*
Special node which describes one variable (space dimension).
*/
class KbVariable : public KbNode
{
private:

	KbVarType Type; // Data type of the column (test string, integer etc.)
	KbComp4 Freq;

public:

	KbVariable( char* NewName, KbVarType NewType );
	~KbVariable();

	// Parameters
	KbVarType GetType();
	KbVarType SetType( KbVarType NewType );
};

/*
Special node which describes one value of variable
*/
class KbValue : public KbNode
{
private:

	KbValType Type;
	KbComp4 Freq;

public:

	KbValue( char* NewName, KbValType NewType );
	~KbValue();

	// Parameters
	KbValType GetType();
	KbValType SetType( KbValType NewType );
};

#endif // !defined(__KbSpace_h__)
