#include "stdafx.h"
#include "..\Lk\Lk.h"
#include "Kb.h"

//
// Construction/Destruction
//

KbSpace::KbSpace( char* NewName ) 
	: KbNode( NewName, KbOperation_And )
{
	VarNumber = 0;

	ValNumber = (LkCoord*) GlobalAlloc( GPTR, (0+1+10) * sizeof(LkCoord) );
	if ( ValNumber == NULL ) ; // ERROR: KbError_Memory
	FillValNumber();

	AccSum = (LkCoord*) GlobalAlloc( GPTR, (0+1+10) * sizeof(LkCoord) );
	if ( AccSum == NULL ) ; // ERROR: KbError_Memory
	ValSum = FillAccSum();

	AccProduct = (KbOffs*) GlobalAlloc( GPTR, (0+1+10) * sizeof(KbOffs) );
	if ( AccProduct == NULL ) ; // ERROR: KbError_Memory
	ValProduct = FillAccProduct();
}

KbSpace::~KbSpace()
{
	if ( ValNumber != NULL )
	{
		GlobalFree( ValNumber );
	}

	if ( AccSum != NULL )
	{
		GlobalFree( AccSum );
	}

	if ( AccProduct != NULL )
	{
		GlobalFree( AccProduct );
	}
}

// Add new variable to the space (append it to the end of the list)
KbError KbSpace::VarAdd( char* VarName )
{
	//
	// First, this variable has to be created (as an object)
	//
	KbVariable* VarNode = new KbVariable( VarName, KbVarType_Null );

	//
	// Then we have to attach it to the list of all variables
	//
	KbError Err = Include( VarNode );

	if ( Err != KbError_Success )
	{
		delete VarNode;
		return Err;
	}

	//
	// Now recalculate the space parameters
	//
	Err = RecalcParams();
	if ( Err != KbError_Success )
	{
		delete VarNode;
		return Err;
	}

	return KbError_Success;
}

// Delete a variable from the space (remove it from the list)
KbError KbSpace::VarDelete( LkCoord VarNo )
{
	KbVariable* VarNode;
	VarNode = (KbVariable*) Coord2Object( VarNo );

	//
	// Detach it from the list of all variables
	//
	KbError Err = Exclude( VarNo );
	if ( Err != KbError_Success )
	{
		return Err;
	}

	//
	// Now, this variable has to be deleted (as an object) along with all values
	//
	delete VarNode;

	//
	// Last, recalculate the space parameters
	//
	Err = RecalcParams();
	if ( Err != KbError_Success )
	{
		return Err;
	}

	return KbError_Success;
}

// Delete a variable from the space (remove it from the list)
KbError KbSpace::VarDelete( char* VarName )
{
	if ( VarName == NULL ) 
		return KbError_General;

	KbError Err;

	LkCoord VarNo = Name2Coord( VarName );
	if ( VarNo == LkCoord_Null ) 
		return KbError_General; // ERROR: No such name

	Err = VarDelete( VarNo );

	return Err;
}

// Get name of the variable given its number. Return the name length
int KbSpace::VarGetName( LkCoord VarNo, char* VarName )
{
	KbNode* VarNode = Coord2Object( VarNo );

	if ( VarNode == NULL ) return -1;

	return VarNode->GetName( VarName );
}

// Get the variable number given its name. 
LkCoord KbSpace::VarGetNo( char* VarName )
{
	return Name2Coord( VarName );
}

// Add new value to the space (append it to the end of the list)
KbError KbSpace::ValAdd( LkCoord VarNo, char* ValName )
{
	KbVariable* VarNode;

	VarNode = (KbVariable*) Coord2Object( VarNo );
	if ( VarNode == NULL )
		return KbError_General; // ERROR: No such variable in the space

	//
	// First, this value has to be created (as an object)
	//
	KbValue* ValNode = new KbValue( ValName, KbValType_Null );

	//
	// Then we have to attach it to the list of all values
	//
	KbError Err = VarNode->Include( ValNode );

	if ( Err != KbError_Success )
	{
		delete ValNode;
		return Err;
	}

	//
	// Now recalculate the space parameters
	//
	Err = RecalcParams();
	if ( Err != KbError_Success )
	{
		delete ValNode;
		return Err;
	}

	return KbError_Success;
}

// Add new value to the space (append it to the end of the list)
KbError KbSpace::ValAdd( char* VarName, char* ValName )
{
	if ( VarName == NULL ) 
		return KbError_General; // ERROR: No such NULL name

	KbError Err;

	LkCoord VarNo = Name2Coord( VarName );
	if ( VarNo == LkCoord_Null ) 
		return KbError_General; // ERROR: No such name

	Err = ValAdd( VarNo, ValName );

	return Err;
}

// Delete a variable value from the space (remove it from the list)
KbError KbSpace::ValDelete( LkCoord VarNo, LkCoord ValNo )
{
	KbVariable* VarNode;
	VarNode = (KbVariable*) Coord2Object( VarNo );

	KbValue* ValNode;
	ValNode = (KbValue*) VarNode->Coord2Object( ValNo );

	//
	// Then we have to detach it from the list of all values
	//
	KbError Err = VarNode->Exclude( ValNo );
	if ( Err != KbError_Success )
	{
		return Err;
	}

	//
	// Now, this variable value has to be deleted (as an object)
	//
	delete ValNode;

	//
	// Last, recalculate the space parameters
	//
	Err = RecalcParams();
	if ( Err != KbError_Success )
	{
		return Err;
	}

	return KbError_Success;
}

// Delete a variable value from the space (remove it from the list)
KbError KbSpace::ValDelete( char* VarName, char* ValName )
{
	if ( VarName == NULL ) 
		return KbError_General;

	if ( ValName == NULL ) 
		return KbError_General;

	KbError Err;

	LkCoord VarNo = Name2Coord( VarName );
	if ( VarNo == LkCoord_Null ) 
		return KbError_General; // ERROR: No such name

	KbVariable* VarNode = (KbVariable*) Coord2Object( VarNo );
	if ( VarNode == NULL ) 
		return KbError_General; // ERROR: No such name

	LkCoord ValNo = VarNode->Name2Coord( ValName );
	if ( ValNo == LkCoord_Null ) 
		return KbError_General; // ERROR: No such name

	Err = ValDelete( VarNo, ValNo );

	return Err;
}

// Get name of the value given its and its variable number. 
// Return the name length
int KbSpace::ValGetName( LkCoord VarNo, LkCoord ValNo, char* ValName )
{
	KbNode* VarNode = Coord2Object( VarNo );
	if ( VarNode == NULL ) 
		return -1;

	KbNode* ValNode = VarNode->Coord2Object( ValNo );
	if ( VarNode == NULL ) 
		return -1;

	return ValNode->GetName( ValName );
}

// Get the value number given its and its variable name.
LkCoord KbSpace::ValGetNo( char* VarName, char* ValName )
{
	KbNode* VarNode = Name2Object( VarName );
	if ( VarNode == NULL ) 
		return -1;

	return VarNode->Name2Coord( ValName );
}

// Fill the vector of the number of values 
LkCoord KbSpace::FillValNumber()
{
	LkCoord var;
	KbNode* VarNode;

	if ( GlobalSize(ValNumber) < (n() + 1) * sizeof(LkCoord) )
	{

		LkCoord* NewMem = (LkCoord*) GlobalReAlloc( HGLOBAL(ValNumber), (n() + 1 + 10) * sizeof(LkCoord), GMEM_MOVEABLE );
		if ( NewMem == NULL )
			return LkCoord_Null; // ERROR: KbError_Memory

		ValNumber = NewMem;
	}

	ValNumber[0] = n();

	for ( var = 0; var < n(); var++ )
	{
		VarNode = Coord2Object( var );
		ValNumber[var+1] = VarNode->n();
	}

	return n();
}

// Fill the vector of the accumulated sum of values 
LkCoord KbSpace::FillAccSum()
{
	LkCoord var;
	KbNode* VarNode;

	if ( GlobalSize(AccSum) < (n() + 1) * sizeof(LkCoord) )
	{
		LkCoord* NewMem = (LkCoord*) GlobalReAlloc( HGLOBAL(AccSum), (n() + 1 + 10) * sizeof(LkCoord), GMEM_MOVEABLE );
		if ( NewMem == NULL )
			return LkCoord_Null; // ERROR: KbError_Memory

		AccSum = NewMem;
	}

	// The very first element contains the number of dimensions (variables)
	AccSum[0] = n();

	if ( n() == 0 ) // No variables -- special case
		return 0;

	AccSum[1] = 0;

	// iterate through all variables and sum their numbers of values
	for ( var = 1; var < n(); var++ )
	{
		VarNode = Coord2Object( var - 1 );
		AccSum[var+1] = AccSum[var] + VarNode->n();
	}

	// Return the sum of all values of variables
	VarNode = Coord2Object( var - 1 );
	return AccSum[var] + VarNode->n();
}

// Fill the vector of the accumulated product of values 
KbOffs KbSpace::FillAccProduct()
{
	LkCoord var;
	KbNode* VarNode;

	if ( GlobalSize(AccProduct) < (n() + 1) * sizeof(KbOffs) )
	{
		KbOffs* NewMem = (KbOffs*) GlobalReAlloc( HGLOBAL(AccProduct), (n() + 1 + 10) * sizeof(KbOffs), GMEM_MOVEABLE );
		if ( NewMem == NULL )
			return KbOffs_Null; // ERROR: KbError_Memory

		AccProduct = NewMem;
	}

	// The very first element contains the number of dimensions (variables)
	AccProduct[0] = n();

	if ( n() == 0 ) // No variables -- special case
		return 1;

	AccProduct[n()] = 1;

	// iterate through all variables and multiply their numbers of values
	for ( var = n(); var > 1; var-- )
	{
		VarNode = Coord2Object( var - 1 );
		if ( VarNode->n() != 0 )
			AccProduct[var-1] = AccProduct[var] * VarNode->n();
		else
			AccProduct[var-1] = AccProduct[var];
	}

	// Return the product of all values of variables
	VarNode = Coord2Object( var - 1 );
	return AccProduct[var] * VarNode->n();
}

// Find the sum of all values of variables 
LkCoord KbSpace::CoordSum()
{
	KbNode* VarNode;
	LkCoord var;
	LkCoord ret = 0;

	// iterate through all variables and sum their numbers of values
	for ( var = 0; var < n(); var++ )
	{
		VarNode = Children[var];
		ret += VarNode->n();
	}

	return ret;
}

// Find the power of the multi-dimensional space 
KbOffs KbSpace::CoordProduct()
{
	KbNode* VarNode;
	LkCoord var;
	KbOffs ret = 1;

	// iterate through all variables and multiply their numbers of values
	for ( var = 0; var < n(); var++ )
	{
		VarNode = Children[var];
		if ( VarNode->n() != 0 )
			ret *= VarNode->n();
	}

	return ret;
}

// Recalculate parameters of the space 
KbError KbSpace::RecalcParams()
{
	LkCoord Ret;

	VarNumber = n();

	Ret = FillValNumber();
	if ( Ret == LkCoord_Null )
		return KbError_Memory;

	ValSum = FillAccSum();
	if ( ValSum == LkCoord_Null )
		return KbError_Memory;

	ValProduct = FillAccProduct();
	if ( ValProduct == KbOffs_Null )
		return KbError_Memory;

	return KbError_Success;
}

// Constructor
KbVariable::KbVariable( char* NewName, KbVarType NewType ) 
	: KbNode( NewName, KbOperation_Or )
{
	Type = NewType;
}

// Destructor
KbVariable::~KbVariable()
{
  // Nothing to do
  ;
}

// Get type of the variable
KbVarType KbVariable::GetType()
{
	return Type;
}

// Set new type of the variable. Note that this operation can 
// influence the values since their type is connected with the
// variable. That is it may result in inconsistencies.
KbVarType KbVariable::SetType( KbVarType NewType )
{
	KbVarType OldType = Type;

	Type = NewType;

	return OldType;
}

// Constructor
KbValue::KbValue( char* NewName, KbValType NewType ) 
	: KbNode( NewName, KbOperation_And )
{
  Type = NewType;
  Freq = 0;
}

// Destructor
KbValue::~KbValue()
{
  // Nothing to do
  ;
}

// Get type of the value
KbValType KbValue::GetType()
{
	return Type;
}

// Set new type of the value. 
KbValType KbValue::SetType( KbValType NewType )
{
	KbValType OldType = Type;

	Type = NewType;

	return OldType;
}
