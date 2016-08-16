#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiCNF                                    >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiCNF::RiCNF( RiSpace* NewSpace, RiOptInduction* NewOptions, char* NewName ) 
    : RiNode( NewName, RiOperation_And ) {
//----------------------------------------------------------------

  if ( NewSpace == NULL ) {
    ; // ERROR:
    }

  Space = NewSpace;

  if ( NewOptions == NULL ) {
    ; // ERROR:
    }

  Options = NewOptions;

	Matrix = NULL;

  Max = 0;
  Min = 0;

  Lines = 0;
  FreeLines = 0;

  }

//================================================================
// Destructor
RiCNF::~RiCNF() {
//----------------------------------------------------------------

  }
