#include "stdafx.h"
#include "Ri.h"

//
// Several helpful macros for this function. They are given the 
// variable number (+1) and return a component from a sectioned vector
//
#define Ptr1Comp(x) (*((RiComp1*)((char*)(ptr1) + CompOffs[x])))
#define Ptr2Comp(x) (*((RiComp1*)((char*)(ptr2) + CompOffs[x])))
// These macros are given offset in bytes (from 0)
#define Ptr1Offs(x) (*((RiComp1*)((char*)(ptr1) + x)))
#define Ptr2Offs(x) (*((RiComp1*)((char*)(ptr2) + x)))

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiCNF_B                                  >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiCNF_B::RiCNF_B( RiSpace* NewSpace, RiOptInduction* NewOptions, char* NewName ) 
    : RiCNF( NewSpace, NewOptions, NewName ) {
//----------------------------------------------------------------

  Lines = 0;
  FreeLines = Options->MaxLines;

  // Here we know the component size specific for the class and can allocate memory
  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp);

  // Add one more line than necessary for a line buffer
  Matrix = (RiSVHeader*) GlobalAlloc( GPTR, (Options->MaxLines+1) * SVLength );
  if ( Matrix == NULL ) ; // ERROR: RiError_Memory

  // Add one more line than necessary for a line buffer
  NewMatrix = (RiSVHeader*) GlobalAlloc( GPTR, (Options->MaxNewLines+1) * SVLength );
  if ( NewMatrix == NULL ) ; // ERROR: RiError_Memory

  // Initialize semantics (add one trivial line)
  Empty();
  }

//================================================================
// Destructor
RiCNF_B::~RiCNF_B() {
//----------------------------------------------------------------

  if ( NewMatrix != NULL ) {
    GlobalFree( NewMatrix );
    }

  if ( Matrix != NULL ) {
    GlobalFree( Matrix );
    }

  }

//================================================================
// Prepare induction process
RiError RiCNF_B::StartInduction() {
//----------------------------------------------------------------

  return Init();
  }

//================================================================
// Finish induction process
RiError RiCNF_B::EndInduction() {
//----------------------------------------------------------------

  if ( Options->GoalVar == RiCoord_Null )
    return RiError_Success;

  //
  // If GoalVar is not Null, remove all disjunctions without the goal set
  //

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiLine DisNo1;
  RiSVHeader* ptr1;
  RiLine NewLines = 0;

  for ( DisNo1 = 0, ptr1 = Matrix; DisNo1 < Lines; DisNo1++, ptr1 = (RiSVHeader*)((char*)(ptr1) + SVLength) ) {

    // if the disjunction has not goal, mark it as absorbed 
    if ( ! (ptr1->Flags & RiSVFlags_IsGoalVar) ) {
      ptr1->Flags |= RiSVFlags_Absorbed; // To be deleted later
      }

    }

  NewLines = ReorderMatrix( Matrix, Lines );

  FreeLines += Lines - NewLines;
  Lines = NewLines;

  return RiError_Success;
  }

//================================================================
// Add one point to the matrix
// It is the main and the most important function in the whole algorithm
RiError RiCNF_B::AddPoint( RiCoord* Point ) {
//----------------------------------------------------------------

  RiCoord x, y;

  //
  // Some useful parameters
  //
  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  // Two arrays to access sectioned vector components
  RiCoord* SectOffs;
  RiCoord* CompOffs;

  SectOffs = (RiCoord*) GlobalAlloc( GPTR, (Space->VarNumber + 1) * sizeof(RiCoord) );
  if ( SectOffs == NULL ) ; // ERROR: RiError_Memory

  CompOffs = (RiCoord*) GlobalAlloc( GPTR, (Space->VarNumber + 1) * sizeof(RiCoord) );
  if ( CompOffs == NULL ) ; // ERROR: RiError_Memory

  SectOffs[0] = Space->VarNumber;
  CompOffs[0] = Space->VarNumber;

  // Initialize these two arrays
  // Calculate offsets (in bytes!) to sections and components of the point
  for ( x = 1; x <= Space->VarNumber; x++ ) {
    SectOffs[x] = sizeof(RiSVHeader) + Space->AccSum[x]*sizeof(RiComp1);
    CompOffs[x] = SectOffs[x] + Point[x]*sizeof(RiComp1);
    }

  // Pointers to disjunctions
  RiSVHeader* ptr1; // Current disjunction in the initial matrix
  RiSVHeader* ptr2; // Next empty location to place new disjunction, i.e., the disjunction being built
  RiComp1* ptr3;

  RiLine DisNo1; // The number of the current disjunction from the initial matrix
  RiLine NewLines = 0;

  char Num0, Num1; // Used when checking section validity and incrementing rank

  // ===
  // Primary Loop !!!
  // Pass through all disjunctions currently in the Matrix, i.e., repeat Lines times
  // ===
  ptr2 = NewMatrix; 
  for ( DisNo1 = 0, ptr1 = Matrix; DisNo1 < Lines; DisNo1++, ptr1 = (RiSVHeader*)((char*)(ptr1) + SVLength) ) {

    //
    // Check if we have a room for VarNumber disjunctions in NewMatrix
    //
    if ( Options->MaxNewLines - NewLines < Space->VarNumber ) {

      // Reorder and delete  the tail lines of the NewMatrix
      NewLines = ReorderMatrix( NewMatrix, NewLines );

      // Check again
      if ( Options->MaxNewLines - NewLines < Space->VarNumber )
        NewLines = Options->MaxLines; // delete

      ptr2 = (RiSVHeader*)((char*)(NewMatrix) + NewLines*SVLength); 
      }

    //
    // Before starting adding the point check if it is absorbed by the disjunction
    //
    for ( x = 1; x <= Space->VarNumber; x++ ) { // loop on components of the vector corresponding to the point coordinates

      // If there is at least one 1 component in the disjunction then the point is absorbed
      if ( Ptr1Comp(x) ) {
        break; // and go to the next source disjunction
        }

      }
    if ( x <= Space->VarNumber ) {

      // The disjunction has not been changed so we do not have to explore it, i.e., go to the next step

      // include one copy of this disjunction in the result set
      CopyMemory ( ptr2, ptr1, SVLength );

      // since this disjunction has not been changed, it cannot be absorbed, so mark it as unabsorbable
      // This flag is checked when trying to absorb disjunctions (e.g., by next ones after adding each point)
      ptr2->OldVar = RiCoord_Null;
      ptr2->OldCompOffs = RiCoord_Null; // Offset in bytes

      // Now we have to check if this disjunction absorbs some previous 
      AbsorbPrev( ptr2, NewMatrix );

      // and include new disjunction in the distribution
      NewLines++;
      ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength);

      continue; // and go to the next source disjunction
      }
    // Now we know that the point is not absorbed by the disjunction

    // ---
    // Secondary loop !!! 
    // Here we generate new disjunctions by adding the point and add them to the matrix
    // ---
    for ( x = 1; x <= Space->VarNumber; x++ ) { // loop on new potentially added disjunctions (not all are included)

      // copy disjunction ptr1 to the next empty location ptr2
      // i.e., to form an initial pattern to modify (to generalize)
      CopyMemory ( ptr2, ptr1, SVLength );

      // build new disjunction by modifying (increasing) one x-th section (componentwise disjunction of two sections)
      // When adding one point (just our case) we have to increase only one component
      // Strictly speaking, we have to take the maximum of these two components, but we know that DistrValue is 1 while our component is 0

      // YES, modify one component !!! Generalize the pattern
      Ptr2Comp(x) = 1;

      // Check if the new pattern is not valid (there has to be at least one 0 component)
      // Try to find 0 component in x-th (just modified) section
      for ( y = 0, ptr3 = (RiComp1*) ((char*) ptr2 + SectOffs[x]); y < Space->ValNumber[x]; y++, ptr3 += 1 ) { // loop on components of x-th section

        if ( *ptr3 == 0 )
          goto IsReduced; // It's okay -- we can proceed (not valid)

        }

      // the disjunction is absolutely valid
      continue;

IsReduced: // Here we know that the disjunction is in reduced form and we can start check for absorption 

      // Remember modified component for optimal absorption
      ptr2->OldVar = x - 1;
      ptr2->OldCompOffs = CompOffs[x]; // Offset in bytes
      if ( Options->GoalVar == x - 1 )
        ptr2->Flags |= RiSVFlags_IsGoalVar; // Set the flag if the new component appeared in the goal section (i.e., it is good disjunction)

      //
      // Increment the new disjunction rank if necesary
      //
      Num1 = 0;
      for ( y = 0, ptr3 = (RiComp1*) ((char*) ptr2 + SectOffs[x]); y < Space->ValNumber[x]; y++, ptr3 += 1 ) { // loop on components of x-th section

        if ( *ptr3 != 0 ) {
          Num1++;
          if ( Num1 == 2 )
            break; // There is the second non-0 component, so the rank has NOT be incremented
          }

        }
      if ( Num1 < 2 ) { // The increased component was the first 1 in this section
        ptr2->Rank++;
        if ( ptr2->Rank > Options->MaxRank )
          continue; // Remove this disjunction -- too high rank
        else if ( ptr2->Rank == Options->MaxRank && Options->GoalVar != RiCoord_Null && ! (ptr2->Flags & RiSVFlags_IsGoalVar) )
          continue; // Remove this disjunction -- the rank is maximal while the goal is not achieved
        }

      //
      // call corresponding procedure of absorption
      //
      AbsorbByPrev( ptr2, NewMatrix );

      // Now we check if it has been absorbed by the previous NewLines disjunctions in NewMatrix
      if ( ptr2->Flags & RiSVFlags_Absorbed ) 
        continue; // If there has been set a flag of absorption then this is bad disjunction (we have more specific one already)

      // After all, include new disjunction in the distribution
      NewLines++;
      ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength);

      } // --- End of secondary loop

    } // === End of primary loop


  NewLines = ReorderMatrix( NewMatrix, NewLines );

  if ( Options->MaxLines < NewLines )
    NewLines = Options->MaxLines;

  CopyMemory ( Matrix, NewMatrix, NewLines*SVLength );

  FreeLines += Lines - NewLines;
  Lines = NewLines;

  GlobalFree( SectOffs );
  GlobalFree( CompOffs );

  return RiError_Success;
  }

//================================================================
// Determine if given disjunction is absorbed by one of the previous 
// disjunctions. If the disjunction is absorbed then its flag is 
// set to TRUE, otherwise it is set to FALSE. All disjunctions with 
// this flag are ignored. 
RiLine RiCNF_B::AbsorbByPrev( RiSVHeader* ptr1, RiSVHeader* ptr2 ) {
//----------------------------------------------------------------

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiCoord z;

  // try to absorb in loop on all not absorbed disjunctions
  for ( ; ptr2 < ptr1; ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength) ) {

    if ( ptr2->Flags & RiSVFlags_Absorbed ) {
      continue; // except for absorbed (absent) disjunctions
      }

    // we suppose that ptr1->OldVar != Null, i.e., it is last pass generalized disjunction

    if ( ptr2->OldVar != RiCoord_Null ) {
      continue; // Only not generalized disjunction is able to absorb us (we are modified)
      }

    // Here we know that ptr2->OldVar == RiCoord_Null
    // so check a condition for this case
    if ( Ptr2Offs(ptr1->OldCompOffs) == 0 ) {
      continue; // It was NOT critical earlier -- to be critical it must be 1
      }

    // Compare the ranks
    if ( ptr1->Rank < ptr2->Rank ) {
      continue; // ptr1 cannot be absorbed by ptr2 since it has less rank
      }


    // Now check componentwise inequality
    for ( z = sizeof(RiSVHeader); z < SVLength; z += sizeof(RiComp1) ) { 

      if ( Ptr1Offs(z) < Ptr2Offs(z) )
        break;

      }
    if ( z < SVLength ) {
      continue;
      }

    // here it means that ptr1 is absorbed by the disjunction ptr2
    ptr1->Flags |= RiSVFlags_Absorbed;
    return 1;
    }

  ptr1->Flags &= (~RiSVFlags_Absorbed);
  return 0;
  }

//================================================================
// Determine if previous disjunctions are absorbed by given 
// disjunctions. If the disjunction is absorbed then its flag is 
// set to TRUE, otherwise it is set to FALSE. All disjunctions with 
// this flag are ignored. 
RiLine RiCNF_B::AbsorbPrev( RiSVHeader* ptr1, RiSVHeader* ptr2 ) {
//----------------------------------------------------------------

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiCoord z;
  RiLine AborbDis = 0;

  // try to absorb in loop on all not absorbed disjunctions
  for ( ; ptr2 < ptr1; ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength) ) {

    if ( ptr2->Flags & RiSVFlags_Absorbed ) {
      continue; // except for absorbed (absent) disjunctions
      }

    if ( ptr2->OldVar == RiCoord_Null ) {
      continue; // non-modified disjunctions cannot be absorbed
      }
    // Here we know that ptr2->OldVar != Null

    // we suppose that ptr1->OldVar == RiCoord_Null, , i.e., it is not generalized disjunction
    // so check a condition for this case

    if ( Ptr1Offs(ptr2->OldCompOffs) == 0 ) {
      continue; // It was NOT critical earlier -- to be critical it must be 1
      }

    // Compare the ranks
    if ( ptr2->Rank < ptr1->Rank ) {
      continue; // ptr2 cannot be absorbed by ptr1 since it has less rank
      }


    // Now check componentwise inequality
    for ( z = sizeof(RiSVHeader); z < SVLength; z += sizeof(RiComp1) ) { 

      if ( Ptr2Offs(z) < Ptr1Offs(z) )
        break;

      }
    if ( z < SVLength ) {
      continue;
      }

    // here it means that ptr2 is absorbed by the disjunction ptr1
    ptr2->Flags |= RiSVFlags_Absorbed;
    AborbDis++;
    }

  return AborbDis;
  }

//================================================================
// Order disjunctions by information and delete unnecessary lines
RiLine RiCNF_B::ReorderMatrix( RiSVHeader* ptr1, RiLine LinesNum ) {
//----------------------------------------------------------------

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiLine DisNo1;
  RiLine DisNo2;
  RiLine AbsorbedDis = 0;
  RiSVHeader* ptr2;
  RiSVHeader* ptr3;
  RiSVHeader* ptrMax;

  //
  // For each disjunction calculate its quantity of infomation and set the corresponding field
  //
  for ( DisNo1 = 0, ptr2 = ptr1; DisNo1 < LinesNum; DisNo1++, ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength) ) {

    // Except for absorbed (marked as valid) disjunctions (the quantity of information is 0)
    if ( ptr2->Flags & RiSVFlags_Absorbed ) {
      ptr2->Rank = RiCoord_Max; // To be placed at the end of the list and deleted
      AbsorbedDis++;
      continue;
      }

    // Except for non-modified disjunctions
    if ( ptr2->OldVar == RiCoord_Null ) {
      continue; // non-modified disjunctions have the same (old) information and rank -- they should not be recalculated
      }

    // Recalculate the quantity of information for the current disjunction
    ptr2->Freq = 1; // Start with the minimal quantity of information (for non-valid disjunctions)
    // ptr2->Rank = 0; // Start with the minimal rank

    // For each non-trivial section calculate the number of 0 and update the quantity of information
    RiCoord z = sizeof(RiSVHeader);

    for ( RiCoord x = 0; x < Space->VarNumber; x++ ) {

      RiCoord Num0 = 0;
      for ( RiCoord y = 0; y < Space->ValNumber[x+1]; y++ ) {

        if ( Ptr2Offs(z) == 0 ) 
          Num0++;

        z++;

        }

      if ( Num0 < Space->ValNumber[x+1] ) { // Take into account only non-trivial sections
        // ptr2->Rank++;
        ptr2->Freq *= Num0;
        }

      }

    if ( ptr2->Freq == 0 )
      AbsorbedDis++; // Something wrong in the generations-absorption procedure

    }

  //
  // Reorder disjunctions by Rank then by Freq
  //

  RiSVHeader* BufDis = (RiSVHeader*)((char*)ptr1 + SVLength*LinesNum);
  for ( DisNo1 = 0, ptr2 = ptr1; DisNo1 < LinesNum; DisNo1++, ptr2 = (RiSVHeader*)((char*)(ptr2) + SVLength) ) {

    // Find maximal, i.e., the disjunction to place at the ptr2 position (instead of ptr2)
    ptrMax = ptr2;
    for ( DisNo2 = DisNo1, ptr3 = ptr2; DisNo2 < LinesNum; DisNo2++, ptr3 = (RiSVHeader*)((char*)(ptr3) + SVLength) ) {
      if ( ptr3->Rank < ptrMax->Rank ) ptrMax = ptr3;
      else if ( ptr3->Rank == ptrMax->Rank )
        if ( ptr3->Freq > ptrMax->Freq ) ptrMax = ptr3;
      }

    // Change ptr3 and ptr1
    if ( ptr2 != ptrMax ) {
      CopyMemory ( BufDis, ptr2, SVLength );
      CopyMemory ( ptr2, ptrMax, SVLength );
      CopyMemory ( ptrMax, BufDis, SVLength );
      }

    }

  return LinesNum - AbsorbedDis;
  }

//================================================================
// Initialize semantics (add one trivial line)
RiError RiCNF_B::Init() {
//----------------------------------------------------------------

  Lines = 1;
  FreeLines = Options->MaxLines - 1;

  //
  // Add one empty disjunction -- a parent of all generated disjunctions
  //
  Matrix->Flags = 0;

  Matrix->Operation = RiOperation_Or;

  Matrix->Rank = 0;
  Matrix->Min = 0;
  Matrix->Max = 1; // It is not so for this disjunction now, however, for all its children it will be so
  Matrix->Freq = RiComp4_Null;

  Matrix->OldVar = RiCoord_Null;
  Matrix->OldCompOffs = RiCoord_Null;
  Matrix->OldComp = RiComp_Null; // not used in boolean version

  // Set all components to 0
  for ( RiCoord z = 0; z < Space->ValSum; z++ ) {
    *((RiComp1*)((char*)Matrix + sizeof(RiSVHeader)) + z) = 0;
    }

  //
  // Set parameters of the whole matrix
  //
  Min = 0;
  Max = 1;
  Freq = RiComp4_Null;

  return RiError_Success;
  }

//================================================================
// Empty semantics (remove all disjunctions)
RiError RiCNF_B::Empty() {
//----------------------------------------------------------------

  Lines = 0;
  FreeLines = Options->MaxLines;

  //
  // Set parameters of the whole matrix
  //
  Min = 0;
  Max = 1;
  Freq = RiComp4_Null;

  return RiError_Success;
  }

//================================================================
// Get text description of the line
int RiCNF_B::TextSV( RiLine LineNo, char* Text ) {
//----------------------------------------------------------------

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  if ( LineNo >= Lines ) {
    // ERROR: wrong line number
    return -1;
    }

  RiSVHeader* ptr1 = (RiSVHeader*)((char*)Matrix + SVLength*LineNo);
  RiComp1* ptr3;

  char buf[2048];
  RiCoord var, val;
  int ret;

  wsprintf( Text, "%i", ptr1->Min );
  ret = lstrlen( Text );

  for ( var = 0; var < Space->VarNumber; var++ ) {

    lstrcat( Text, "." ); // Each section starts with '.'
    ret += lstrlen( "." );

    ptr3 = (RiComp1*) ((char*) ptr1 + sizeof(RiSVHeader) + Space->AccSum[var+1]*sizeof(RiComp1));
    for ( val = 0; val < Space->ValNumber[var+1]; val++, ptr3 += 1 ) { // loop on components of var-th section

      wsprintf( buf, "%i", (*ptr3 == 0) ? 0 : 1 );

      lstrcat( Text, buf );
      ret += lstrlen( buf );

      }

    }

  return ret;
  }

//================================================================
// Get text description of the line
int RiCNF_B::Text( RiLine LineNo, char* RetText ) {
//----------------------------------------------------------------

  if ( LineNo >= Lines ) {
    // ERROR: wrong line number
    return -1;
    }

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  char buf[8192];
  RiCoord var;
  int ret;

  RiCoord GoalVar = Options->GoalVar; // Later this parameter should be taken from view options (although view options can be initialized from induction options)
  if ( GoalVar >= Space->VarNumber ) {
    GoalVar = RiCoord_Null; // Something wrong
    }

  lstrcpy( RetText, "" );
  ret = lstrlen( RetText );

  //
  // Look through all sections and form IF part
  //
  for ( var = 0; var < Space->VarNumber; var++ ) {

    // Except for the goal variable -- it will be in THEN part (end of rule)
    if ( var == GoalVar )
      continue;

    Text( LineNo, var, buf );

    if ( lstrlen(buf) == 0 )
      continue;

    lstrcat( RetText, buf );
    ret += lstrlen( buf );

    lstrcat( RetText, " AND " );
    ret += lstrlen( " AND " );

    }

  if ( lstrlen(RetText) != 0 ) {
    // Remove last AND
    RetText[ lstrlen(RetText) - lstrlen("AND ")] = 0;
    ret -= lstrlen( "AND " );
    }

  // and add THEN instead of it
  lstrcat( RetText, "THEN " );
  ret += lstrlen( "THEN " );

  //
  // Process the goal section if any
  //

  Text( LineNo, GoalVar, buf );

  lstrcat( RetText, buf );
  ret += lstrlen( buf );

  return ret;
  }

//================================================================
// Get text description of one variable
int RiCNF_B::Text( RiLine LineNo, RiCoord VarNo, char* RetText ) {
//----------------------------------------------------------------

  if ( LineNo >= Lines ) {
    return -1; // ERROR: wrong line number
    }

  if ( VarNo != RiCoord_Null && VarNo >= Space->VarNumber ) {
    return -1; // ERROR: wrong variable number
    }

  char buf[4096];
  RiCoord val;
  int ret;

  RiCoord GoalVar = Options->GoalVar; // Later this parameter should be taken from view options (although view options can be initialized from induction options)
  if ( GoalVar >= Space->VarNumber ) {
    GoalVar = RiCoord_Null; // Something wrong
    }

  lstrcpy( RetText, "" );
  ret = lstrlen( RetText );


  if ( VarNo == GoalVar ) {
    //
    // No inverting
    //

    if ( VarNo == RiCoord_Null || IsEmpty( LineNo, VarNo ) ) {

      lstrcat( RetText, "Contradiction (incompatible values)" );
      ret += lstrlen( "Contradiction (incompatible values)" );

      }
    else {

      // Get and write the variable name
      Space->VarGetName( VarNo, buf );
      lstrcat( RetText, buf );
      ret += lstrlen( buf );

      lstrcat( RetText, " = {" );
      ret += lstrlen( " = {" );

      // Write only non-trivial values (1 components -- without inversion)

      for ( val = 0; val < Space->ValNumber[VarNo+1]; val++ ) { // loop on components of var-th section

        // Check if it is 0 component, otherwise continue
        if ( GetComp( LineNo, VarNo, val ) == 0 )
          continue; // Go to the next component

        Space->ValGetName( VarNo, val, buf );
        lstrcat( RetText, buf );
        ret += lstrlen( buf );

        lstrcat( RetText, " OR " );
        ret += lstrlen( " OR " );

        }

      // Remove last OR
      RetText[ lstrlen(RetText) - lstrlen(" OR ")] = 0;
      ret -= lstrlen( " OR " );

      // and write } instead of it
      lstrcat( RetText, "}" );
      ret += lstrlen( "}" );

      }

    }
  else {
    //
    // Invert the semantics
    //

    if ( VarNo == RiCoord_Null || IsEmpty( LineNo, VarNo ) ) {

      // lstrcat( RetText, "Valid (no constraints) " );
      // ret += lstrlen( "Valid (no constraints) " );

      }
    else {

      // Get and write the variable name
      Space->VarGetName( VarNo, buf );
      lstrcat( RetText, buf );
      ret += lstrlen( buf );

      lstrcat( RetText, " = {" );
      ret += lstrlen( " = {" );

      // Write only inverted trivial values (0 components with inversion to 1)

      for ( val = 0; val < Space->ValNumber[VarNo+1]; val++ ) { // loop on components of var-th section

        // Check if it is 0 component, otherwise continue
        if ( GetComp( LineNo, VarNo, val ) != 0 )
          continue; // Go to the next component

        Space->ValGetName( VarNo, val, buf );
        lstrcat( RetText, buf );
        ret += lstrlen( buf );

        lstrcat( RetText, " OR " );
        ret += lstrlen( " OR " );

        }

      // Remove last OR
      RetText[ lstrlen(RetText) - lstrlen(" OR ")] = 0;
      ret -= lstrlen( " OR " );

      // and write } instead of it
      lstrcat( RetText, "}" );
      ret += lstrlen( "}" );

      }

    }

  return ret;
  }

//================================================================
// Determine if the section of the rule is empty (trivial)
BOOL RiCNF_B::IsEmpty( RiLine LineNo, RiCoord VarNo ) {
//----------------------------------------------------------------

  if ( LineNo >= Lines ) {
    // ERROR: wrong line number
    return TRUE;
    }

  if ( VarNo >= Space->VarNumber ) {
    // ERROR: wrong variable number
    return TRUE;
    }

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiSVHeader* ptr1 = (RiSVHeader*)((char*)Matrix + SVLength*LineNo);
  RiComp1* ptr3 = (RiComp1*) ((char*) ptr1 + sizeof(RiSVHeader) + Space->AccSum[VarNo+1]*sizeof(RiComp1));
  RiCoord val;

  // Check if the section is trivial (empty)
  for ( val = 0; val < Space->ValNumber[VarNo+1]; val++, ptr3 += 1 ) { // loop on components of VarNo-th section

    if ( *ptr3 != 0 )
      break;

    }
  if ( val == Space->ValNumber[VarNo+1] )
    return TRUE; // The section consists of all 0 so the conclusion is not interesting (empty)

  return FALSE;
  }

//================================================================
// Get the component value (return RiComp4 or RiCompD !!!)
RiComp1 RiCNF_B::GetComp( RiLine LineNo, RiCoord VarNo, RiCoord ValNo ) {
//----------------------------------------------------------------

  if ( LineNo >= Lines ) {
    // ERROR: wrong line number
    return 0;
    }

  if ( VarNo >= Space->VarNumber ) {
    // ERROR: wrong variable number
    return 0;
    }

  if ( ValNo >= Space->ValNumber[VarNo + 1] ) {
    // ERROR: wrong value number
    return 0;
    }

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiSVHeader* ptr1 = (RiSVHeader*)((char*)Matrix + SVLength*LineNo);
  RiComp1* ptr3 = (RiComp1*) ((char*) ptr1 + sizeof(RiSVHeader) + Space->AccSum[VarNo+1]*sizeof(RiComp1));

  return ptr3[ValNo];
  }

//================================================================
// Set the component value (return old component RiComp4 or RiCompD !!!)
RiComp1 RiCNF_B::SetComp( RiLine LineNo, RiCoord VarNo, RiCoord ValNo, RiComp1 Comp ) {
//----------------------------------------------------------------

  if ( LineNo >= Lines ) {
    // ERROR: wrong line number
    return 0;
    }

  if ( VarNo >= Space->VarNumber ) {
    // ERROR: wrong variable number
    return 0;
    }

  if ( ValNo >= Space->ValNumber[VarNo + 1] ) {
    // ERROR: wrong value number
    return 0;
    }

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp1);

  RiSVHeader* ptr1 = (RiSVHeader*)((char*)Matrix + SVLength*LineNo);
  RiComp1* ptr3 = (RiComp1*) ((char*) ptr1 + sizeof(RiSVHeader) + Space->AccSum[VarNo+1]*sizeof(RiComp1));

  RiComp1 OldComp = ptr3[ValNo];

  ptr3[ValNo] = Comp;

  return OldComp;
  }

