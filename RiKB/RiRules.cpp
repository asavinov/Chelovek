#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiRules                                  >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiRules::RiRules( RiSpace* NewSpace, char* NewName ) : RiNode( NewName, RiOperation_Null ) {
//----------------------------------------------------------------

  if ( NewSpace == NULL ) ; // ERROR:
  Space = NewSpace;

  RiCoord SVLength = sizeof(RiSVHeader) + Space->ValSum*sizeof(RiComp4);
  Matrix = (RiSVHeader*) GlobalAlloc( GPTR, 1024 * SVLength );
  if ( Matrix == NULL ) ; // ERROR: RiError_Memory

  Max = 0;
  Min = 0;

  LinesNumber = 0;
  FreeLinesNumber = 1024;
  }

//================================================================
// Destructor
RiRules::~RiRules() {
//----------------------------------------------------------------

  if ( Matrix != NULL ) {
    GlobalFree( Matrix );
    }

  }



