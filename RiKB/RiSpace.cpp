#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiSpace                                  >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor
RiSpace::RiSpace( char* NewName ) 
     : RiNode( NewName, RiOperation_And ) {
//----------------------------------------------------------------

  VarNumber = 0;

  ValNumber = (RiCoord*) GlobalAlloc( GPTR, (0+1+10) * sizeof(RiCoord) );
  if ( ValNumber == NULL ) ; // ERROR: RiError_Memory
  FillValNumber();

  AccSum = (RiCoord*) GlobalAlloc( GPTR, (0+1+10) * sizeof(RiCoord) );
	if ( AccSum == NULL ) ; // ERROR: RiError_Memory
  ValSum = FillAccSum();

  AccProduct = (RiOffs*) GlobalAlloc( GPTR, (0+1+10) * sizeof(RiOffs) );
	if ( AccProduct == NULL ) ; // ERROR: RiError_Memory
  ValProduct = FillAccProduct();

  }

//================================================================
// Destructor
RiSpace::~RiSpace() {
//----------------------------------------------------------------

  if ( ValNumber != NULL ) {
    GlobalFree( ValNumber );
    }

  if ( AccSum != NULL ) {
    GlobalFree( AccSum );
    }

  if ( AccProduct != NULL ) {
    GlobalFree( AccProduct );
    }

  }

//================================================================
// Add new variable to the space (append it to the end of the list)
RiError RiSpace::VarAdd( char* VarName ) {
//----------------------------------------------------------------

  //
  // First, this variable has to be created (as an object)
  //
  RiVariable* VarNode = new RiVariable( VarName, RiVarType_Null );

  //
  // Then we have to attach it to the list of all variables
  //
  RiError Err = Include( VarNode );

  if ( Err != RiError_Success ) {
    delete VarNode;
    return Err;
    }

  //
  // Now recalculate the space parameters
  //
  Err = RecalcParams();
  if ( Err != RiError_Success ) {
    delete VarNode;
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Delete a variable from the space (remove it from the list)
RiError RiSpace::VarDelete( RiCoord VarNo ) {
//----------------------------------------------------------------

  RiVariable* VarNode;
  VarNode = (RiVariable*) Coord2Object( VarNo );

  //
  // Detach it from the list of all variables
  //
  RiError Err = Exclude( VarNo );
  if ( Err != RiError_Success ) {
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
  if ( Err != RiError_Success ) {
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Delete a variable from the space (remove it from the list)
RiError RiSpace::VarDelete( char* VarName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General;

  RiError Err;

  RiCoord VarNo = Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  Err = VarDelete( VarNo );

  return Err;
  }

//================================================================
// Get name of the variable given its number. Return the name length
int RiSpace::VarGetName( RiCoord VarNo, char* VarName ) {
//----------------------------------------------------------------

  RiNode* VarNode = Coord2Object( VarNo );

  if ( VarNode == NULL ) return -1;

  return VarNode->GetName( VarName );
  }

//================================================================
// Get the variable number given its name. 
RiCoord RiSpace::VarGetNo( char* VarName ) {
//----------------------------------------------------------------

  return Name2Coord( VarName );

  }

//================================================================
// Add new value to the space (append it to the end of the list)
RiError RiSpace::ValAdd( RiCoord VarNo, char* ValName ) {
//----------------------------------------------------------------

  RiVariable* VarNode;

  VarNode = (RiVariable*) Coord2Object( VarNo );
  if ( VarNode == NULL )
    return RiError_General; // ERROR: No such variable in the space

  //
  // First, this value has to be created (as an object)
  //
  RiValue* ValNode = new RiValue( ValName, RiValType_Null );

  //
  // Then we have to attach it to the list of all values
  //
  RiError Err = VarNode->Include( ValNode );

  if ( Err != RiError_Success ) {
    delete ValNode;
    return Err;
    }

  //
  // Now recalculate the space parameters
  //
  Err = RecalcParams();
  if ( Err != RiError_Success ) {
    delete ValNode;
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Add new value to the space (append it to the end of the list)
RiError RiSpace::ValAdd( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General; // ERROR: No such NULL name

  RiError Err;

  RiCoord VarNo = Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  Err = ValAdd( VarNo, ValName );

  return Err;
  }

//================================================================
// Delete a variable value from the space (remove it from the list)
RiError RiSpace::ValDelete( RiCoord VarNo, RiCoord ValNo ) {
//----------------------------------------------------------------

  RiVariable* VarNode;
  VarNode = (RiVariable*) Coord2Object( VarNo );

  RiValue* ValNode;
  ValNode = (RiValue*) VarNode->Coord2Object( ValNo );

  //
  // Then we have to detach it from the list of all values
  //
  RiError Err = VarNode->Exclude( ValNo );
  if ( Err != RiError_Success ) {
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
  if ( Err != RiError_Success ) {
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Delete a variable value from the space (remove it from the list)
RiError RiSpace::ValDelete( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General;

  if ( ValName == NULL ) 
    return RiError_General;

  RiError Err;

  RiCoord VarNo = Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  RiVariable* VarNode = (RiVariable*) Coord2Object( VarNo );
  if ( VarNode == NULL ) 
    return RiError_General; // ERROR: No such name

  RiCoord ValNo = VarNode->Name2Coord( ValName );
  if ( ValNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  Err = ValDelete( VarNo, ValNo );

  return Err;
  }

//================================================================
// Get name of the value given its and its variable number. 
// Return the name length
int RiSpace::ValGetName( RiCoord VarNo, RiCoord ValNo, char* ValName ) {
//----------------------------------------------------------------

  RiNode* VarNode = Coord2Object( VarNo );
  if ( VarNode == NULL ) 
    return -1;

  RiNode* ValNode = VarNode->Coord2Object( ValNo );
  if ( VarNode == NULL ) 
    return -1;

  return ValNode->GetName( ValName );
  }

//================================================================
// Get the value number given its and its variable name.
RiCoord RiSpace::ValGetNo( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  RiNode* VarNode = Name2Object( VarName );
  if ( VarNode == NULL ) 
    return -1;

  return VarNode->Name2Coord( ValName );
  }



//================================================================
// Fill the vector of the number of values 
RiCoord RiSpace::FillValNumber() {
//----------------------------------------------------------------
  RiCoord var;
  RiNode* VarNode;

  if ( GlobalSize(ValNumber) < (n() + 1) * sizeof(RiCoord) ) {

    RiCoord* NewMem = (RiCoord*) GlobalReAlloc( HGLOBAL(ValNumber), (n() + 1 + 10) * sizeof(RiCoord), GMEM_MOVEABLE );
    if ( NewMem == NULL )
      return RiCoord_Null; // ERROR: RiError_Memory

    ValNumber = NewMem;
    }

  ValNumber[0] = n();

  for ( var = 0; var < n(); var++ ) {

    VarNode = Coord2Object( var );
    ValNumber[var+1] = VarNode->n();

    }

  return n();
  }

//================================================================
// Fill the vector of the accumulated sum of values 
RiCoord RiSpace::FillAccSum() {
//----------------------------------------------------------------
  RiCoord var;
  RiNode* VarNode;

  if ( GlobalSize(AccSum) < (n() + 1) * sizeof(RiCoord) ) {

    RiCoord* NewMem = (RiCoord*) GlobalReAlloc( HGLOBAL(AccSum), (n() + 1 + 10) * sizeof(RiCoord), GMEM_MOVEABLE );
    if ( NewMem == NULL )
      return RiCoord_Null; // ERROR: RiError_Memory

    AccSum = NewMem;
    }

  // The very first element contains the number of dimensions (variables)
  AccSum[0] = n();

  if ( n() == 0 ) // No variables -- special case
    return 0;

  AccSum[1] = 0;

  // iterate through all variables and sum their numbers of values
  for ( var = 1; var < n(); var++ ) {

    VarNode = Coord2Object( var - 1 );
    AccSum[var+1] = AccSum[var] + VarNode->n();

    }

  // Return the sum of all values of variables
  VarNode = Coord2Object( var - 1 );
  return AccSum[var] + VarNode->n();
  }

//================================================================
// Fill the vector of the accumulated product of values 
RiOffs RiSpace::FillAccProduct() {
//----------------------------------------------------------------
  RiCoord var;
  RiNode* VarNode;

  if ( GlobalSize(AccProduct) < (n() + 1) * sizeof(RiOffs) ) {

    RiOffs* NewMem = (RiOffs*) GlobalReAlloc( HGLOBAL(AccProduct), (n() + 1 + 10) * sizeof(RiOffs), GMEM_MOVEABLE );
    if ( NewMem == NULL )
      return RiOffs_Null; // ERROR: RiError_Memory

    AccProduct = NewMem;
    }

  // The very first element contains the number of dimensions (variables)
  AccProduct[0] = n();

  if ( n() == 0 ) // No variables -- special case
    return 1;

  AccProduct[n()] = 1;

  // iterate through all variables and multiply their numbers of values
  for ( var = n(); var > 1; var-- ) {

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

//================================================================
// Find the sum of all values of variables 
RiCoord RiSpace::CoordSum() {
//----------------------------------------------------------------
  RiNode* VarNode;
  RiCoord var;
  RiCoord ret = 0;

  // iterate through all variables and sum their numbers of values
  for ( var = 0; var < n(); var++ ) {

    VarNode = Children[var];
    ret += VarNode->n();

    }

  return ret;
  }

//================================================================
// Find the power of the multi-dimensional space 
RiOffs RiSpace::CoordProduct() {
//----------------------------------------------------------------
  RiNode* VarNode;
  RiCoord var;
  RiOffs ret = 1;

  // iterate through all variables and multiply their numbers of values
  for ( var = 0; var < n(); var++ ) {

    VarNode = Children[var];
    if ( VarNode->n() != 0 )
      ret *= VarNode->n();

    }

  return ret;
  }

//================================================================
// Recalculate parameters of the space 
RiError RiSpace::RecalcParams() {
//----------------------------------------------------------------

  RiCoord Ret;

  VarNumber = n();

  Ret = FillValNumber();
  if ( Ret == RiCoord_Null )
    return RiError_Memory;

  ValSum = FillAccSum();
  if ( ValSum == RiCoord_Null )
    return RiError_Memory;

  ValProduct = FillAccProduct();
  if ( ValProduct == RiOffs_Null )
    return RiError_Memory;


  return RiError_Success;
  }



//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiVariable                               >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor
RiVariable::RiVariable( char* NewName, RiVarType NewType ) 
     : RiNode( NewName, RiOperation_Or ) {
//----------------------------------------------------------------

  Type = NewType;

  }

//================================================================
// Destructor
RiVariable::~RiVariable() {
//----------------------------------------------------------------

  // Nothing to do
  ;

  }

//================================================================
// Get type of the variable
RiVarType RiVariable::GetType() {
//----------------------------------------------------------------
  return Type;
  }

//================================================================
// Set new type of the variable. Note that this operation can 
// influence the values since their type is connected with the
// variable. That is it may result in inconsistencies.
RiVarType RiVariable::SetType( RiVarType NewType ) {
//----------------------------------------------------------------

  RiVarType OldType = Type;

	Type = NewType;

  return OldType;
  }



//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiValue                                  >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor
RiValue::RiValue( char* NewName, RiValType NewType ) 
     : RiNode( NewName, RiOperation_And ) {
//----------------------------------------------------------------

  Type = NewType;
  Freq = 0;

  }

//================================================================
// Destructor
RiValue::~RiValue() {
//----------------------------------------------------------------

  // Nothing to do
  ;

  }

//================================================================
// Get type of the value
RiValType RiValue::GetType() {
//----------------------------------------------------------------
  return Type;
  }

//================================================================
// Set new type of the value. 
RiValType RiValue::SetType( RiValType NewType ) {
//----------------------------------------------------------------

  RiValType OldType = Type;

	Type = NewType;

  return OldType;
  }


