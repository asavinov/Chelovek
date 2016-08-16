#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiKB                                     >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiKB::RiKB( char* NewName ) : RiNode( NewName, RiOperation_Null ) {
//----------------------------------------------------------------

  //
  // Set default options
  //
  OptInduction.MaxNewLines = 2000; // How many to delete when a limitation fires
  OptInduction.MaxLines = 200; // Do not generate more patterns
  OptInduction.MaxRank = 3; // Do not generate and evaluate patterns with higher rank
  OptInduction.GoalVar = RiCoord_Null; // Generate only patterns involving this variable

  OptAnalysis.MaxValues = 100;

  // Process state
  Process = RiProcess_Null;

  // Main objects
  Space = new RiSpace( "Syntax" );
  Rules = new RiCNF_B( Space, & OptInduction, "Semantics" );
  DataSource = NULL; // No default data source, i.e., empty data source is equivalent to its absence

  }

//================================================================
// Destructor
RiKB::~RiKB() {
//----------------------------------------------------------------

  if ( Rules != NULL ) {
    delete Rules;
    }

  if ( Space != NULL ) {
    delete Space;
    }

  if ( DataSource != NULL ) {
    delete DataSource;
    }
  }

//================================================================
// Begin the process. This turns the KB into a special state when 
// it is prepared to conduct certain actions.
RiError RiKB::ProcessBegin( RiProcess NewProcess ) {
//----------------------------------------------------------------

  // This function can be called only from the Null process state 
  if ( Process != RiProcess_Null )
    return RiError_General; // ERROR: Already in process

  //
  // Depending on the concrete process make specific preparations 
  //
  switch (NewProcess) {

  case RiProcess_FindPatterns :
    Process = RiProcess_FindPatterns;
    FindPatternsBegin();
    break;

  case RiProcess_FillPatterns :
    Process = RiProcess_FillPatterns;
    break;

  default:
    return RiError_General; // ERROR: No such a process

  }

  return RiError_Success;
  }

//================================================================
// End the process. This turns the KB into the Null process state 
RiError RiKB::ProcessEnd() {
//----------------------------------------------------------------

  if ( Process == RiProcess_Null )
    return RiError_General; // ERROR: Already in Null process state

  //
  // Depending on the current process state make specific operations
  //
  switch (Process) {

  case RiProcess_FindPatterns:
    FindPatternsEnd();
    break;

  case RiProcess_FillPatterns:
    break;

  default:
    return RiError_General; // ERROR: No such a process

  }

  Process = RiProcess_Null;

  return RiError_Success;
  }

//================================================================
// Process one data point
RiError RiKB::ProcessPoint( RiCoord* Point, RiComp Component ) {
//----------------------------------------------------------------

  //
  // Depending on the current process call specific function
  //
  switch (Process) {

  case RiProcess_Null: 
    return RiError_General; // ERROR: Must be in process

  case RiProcess_FindPatterns :
    // Add point to the CNF
    break;

  case RiProcess_FillPatterns :
    break;

  default:
    return RiError_General; // ERROR: No such a process

  }

  return RiError_Success;
  }

//================================================================
// Begin the process of finding patters and prepare all data structures
RiError RiKB::FindPatternsBegin() {
//----------------------------------------------------------------

  return RiError_Success;
  }

//================================================================
// End the process of finding patters and destroy all data structures
RiError RiKB::FindPatternsEnd() {
//----------------------------------------------------------------

  // Empty current semantcis (delete all rules)


  // Transform patterns into rules


  // Delete CNF


  return RiError_Success;
  }

//================================================================
// Get the number of variables (dimension)
RiCoord RiKB::VarNumber() {
//----------------------------------------------------------------

  return Space->VarNumber;

  }

//================================================================
// Add new variable to the space (append it to the end of the list)
RiError RiKB::VarAdd( char* VarName ) {
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
RiError RiKB::VarDelete( RiCoord VarNo ) {
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
RiError RiKB::VarDelete( char* VarName ) {
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
int RiKB::VarGetName( RiCoord VarNo, char* VarName ) {
//----------------------------------------------------------------

  return Space->VarGetName( VarNo, VarName );
  }

//================================================================
// Get the variable number given its name. 
RiCoord RiKB::VarGetNo( char* VarName ) {
//----------------------------------------------------------------

  return Space->VarGetNo( VarName );
  }

//================================================================
// Get the number of values for one variable
RiCoord RiKB::ValNumber( RiCoord VarNo ) {
//----------------------------------------------------------------

  return Space->ValNumber[VarNo+1];
  }

//================================================================
// Get the number of values for one variable
RiCoord RiKB::ValNumber( char* VarName ) {
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
RiError RiKB::ValAdd( RiCoord VarNo, char* ValName ) {
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
RiError RiKB::ValAdd( char* VarName, char* ValName ) {
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
RiError RiKB::ValDelete( RiCoord VarNo, RiCoord ValNo ) {
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
RiError RiKB::ValDelete( char* VarName, char* ValName ) {
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
int RiKB::ValGetName( RiCoord VarNo, RiCoord ValNo, char* ValName ) {
//----------------------------------------------------------------

  return Space->ValGetName( VarNo, ValNo, ValName );
  }

//================================================================
// Get the value number given its and its variable name.
RiCoord RiKB::ValGetNo( char* VarName, char* ValName ) {
//----------------------------------------------------------------

  return Space->ValGetNo( VarName, ValName );
  }
