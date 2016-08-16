#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< Global Functions                         >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Find how many pointers are in NULL terminated array
RiCoord PointerArrayLength( RiNode** ArrayOfPointers ) {
//----------------------------------------------------------------

  for ( RiCoord x = 0; ArrayOfPointers[x] != NULL; x++ ) ;

  return x;
  }

//================================================================
// Find how many pointers are in NULL terminated array
RiCoord Pointer2Index( RiNode** ArrayOfPointers, RiNode* Pointer ) {
//----------------------------------------------------------------

  // Scan through the list of pointers and find given pointer
  for ( RiCoord x = 0; (ArrayOfPointers[x] != Pointer) && (ArrayOfPointers[x] != NULL); x++ ) ;

  return x;
  }

//================================================================
// Fill the linear array of coordinates given an offset in the linear space
// The fist elements in coordinates is the number of dimensions (the array length)
long Offset2Coordinates( long* AP_Array, long Offset, RiCoord* Coordinates ) {
//----------------------------------------------------------------

  RiCoord n = Coordinates[0];

  // iterate through all dimensions and update the offset
  for ( RiCoord x = 1; x <= n; x++ ) {
    Coordinates[x] = Offset / AP_Array[x];
    Offset = Offset % AP_Array[x];
    }

  return Offset;
  }

//================================================================
// Find offset in the linear array corresponding to given coordinates
// The fist elements in coordinates is the number of dimensions (the array length)
long Coordinates2Offset( long* AP_Array, RiCoord* Coordinates ) {
//----------------------------------------------------------------

  RiCoord n = Coordinates[0];
  long ret = 0;

  // iterate through all dimensions and update the offset
  for ( RiCoord x = 1; x <= n; x++ ) {
    ret += (Coordinates[x]*AP_Array[x]);
    }

  return ret;
  }


