#include "stdafx.h"
#include "..\Lk\Lk.h"
#include "Kb.h"

KbKB::KbKB( char* Name ) 
	: LkNode( Name )
{
	//
	// Set default options
	//
	OptInduction.MaxNewLines = 2000; // How many to delete when a limitation fires
	OptInduction.MaxLines = 200; // Do not generate more patterns
	OptInduction.MaxRank = 3; // Do not generate and evaluate patterns with higher rank
	OptInduction.GoalVar = RiCoord_Null; // Generate only patterns involving this variable

	OptAnalysis.MaxValues = 100;

	// Main objects
	DataSource = new KbDataSourceCSV( "Data Source" );
	Space = new RiSpace( "Syntax" );
	Rules = new RiCNF_B( Space, & OptInduction, "Semantics" );
}

KbKB::~KbKB()
{
	if ( DataSource != NULL )
	{
		delete DataSource;
	}

	if ( Rules != NULL )
	{
		delete Rules;
	}

	if ( Space != NULL )
	{
		delete Space;
	}

}





//================================================================
// Get the number of variables (dimension)
RiCoord KbKB::VarNumber() {
//----------------------------------------------------------------

  return Space->VarNumber;

  }

//================================================================
// Add new variable to the space (append it to the end of the list)
RiError KbKB::VarAdd( char* VarName ) {
//----------------------------------------------------------------

  RiError Err = Space->VarAdd( VarName );

  if ( Err != RiError_Success ) {
    return Err;
    }

  //
  // And last, we have to update the rule base somehow 
  //

  // todo !!!

  return RiError_Success;
  }

//================================================================
// Delete a variable from the space (remove it from the list)
RiError KbKB::VarDelete( RiCoord VarNo ) {
//----------------------------------------------------------------

  //
  // First, we have to update the rule base somehow 
  //

  // todo !!!

  RiError Err = Space->VarDelete( VarNo );

  if ( Err != RiError_Success ) {
    // Undo the change of the rule base -- todo !!!
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Delete a variable from the space (remove it from the list)
RiError KbKB::VarDelete( char* VarName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General;

  RiError Err;

  RiCoord VarNo = Space->Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  Err = VarDelete( VarNo );

  return Err;
  }

//================================================================
// Get name of the variable given its number. Return the name length
int KbKB::VarGetName( RiCoord VarNo, char* VarName ) {
//----------------------------------------------------------------

  return Space->VarGetName( VarNo, VarName );
  }

//================================================================
// Get the variable number given its name. 
RiCoord KbKB::VarGetNo( char* VarName ) {
//----------------------------------------------------------------

  return Space->VarGetNo( VarName );
  }

//================================================================
// Get the number of values for one variable
RiCoord KbKB::ValNumber( RiCoord VarNo ) {
//----------------------------------------------------------------

  return Space->ValNumber[VarNo+1];
  }

//================================================================
// Get the number of values for one variable
RiCoord KbKB::ValNumber( char* VarName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiCoord_Null;

  RiCoord VarNo = Space->Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  return Space->ValNumber[VarNo+1];
  }

//================================================================
// Add new value to the space (append it to the end of the list)
RiError KbKB::ValAdd( RiCoord VarNo, char* ValName ) {
//----------------------------------------------------------------

  RiError Err;

  Err = Space->ValAdd( VarNo, ValName );
  if ( Err != RiError_Success ) {
    return Err;
    }

  //
  // And last, we have to update the rule base somehow 
  //

  // todo !!!

  return RiError_Success;
  }

//================================================================
// Add new value to the space (append it to the end of the list)
RiError KbKB::ValAdd( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General; // ERROR: No such NULL name

  RiError Err;

  RiCoord VarNo = Space->Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  Err = ValAdd( VarNo, ValName );

  return Err;
  }

//================================================================
// Delete a variable value from the space (remove it from the list)
RiError KbKB::ValDelete( RiCoord VarNo, RiCoord ValNo ) {
//----------------------------------------------------------------

  //
  // First, we have to update the rule base somehow 
  //

  // todo !!!

  //
  // Last, recalculate the space parameters
  //
  RiError Err = Space->ValDelete( VarNo, ValNo );
  if ( Err != RiError_Success ) {
    // Undo the change of the rule base
    return Err;
    }

  return RiError_Success;
  }

//================================================================
// Delete a variable value from the space (remove it from the list)
RiError KbKB::ValDelete( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  if ( VarName == NULL ) 
    return RiError_General;

  if ( ValName == NULL ) 
    return RiError_General;

  RiError Err;

  RiCoord VarNo = Space->Name2Coord( VarName );
  if ( VarNo == RiCoord_Null ) 
    return RiError_General; // ERROR: No such name

  RiVariable* VarNode = (RiVariable*) Space->Coord2Object( VarNo );
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
int KbKB::ValGetName( RiCoord VarNo, RiCoord ValNo, char* ValName ) {
//----------------------------------------------------------------

  return Space->ValGetName( VarNo, ValNo, ValName );
  }

//================================================================
// Get the value number given its and its variable name.
RiCoord KbKB::ValGetNo( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  return Space->ValGetNo( VarName, ValName );
  }
