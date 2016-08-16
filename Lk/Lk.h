#define _DIAG // Gather diagnostic/statistic information

#define LN_2 0.69314718055994530942   /* ln(2) */

//================================================================
// LkCoord
//================================================================
// Each variable takes some number of values. In addition, 
// there is some number of variables themselves. Generally, 
// the number of either variables or values is reffered to 
// as coordinate. Coordinate is the number of children for 
// a parent. For example, if the parent is some variable,
// then each its value has some number and hence coordinate.

// This type defines restrictions on the possible number of
// both variables and values of variables

typedef unsigned long LkCoord;

#define LkCoord_Max   0xfffffff0 // maximal value of variables
#define LkCoord_Min   0x00000000 // minimal value of variables
#define LkCoord_Null  0xfffffff0 + 1 // NULL value for coordinates
#define LkCoord_All   0xfffffff0 + 2 // all coordinates
#define LkCoord_Any   0xfffffff0 + 3 // Any coordinate (unknown)
#define LkCoord_MinusInf 0xfffffff0 + 4 // Minus infinity
#define LkCoord_PlusInf  0xfffffff0 + 5 // Plus infinity
#define LkCoord_Inf      0xfffffff0 + 6 // Infinity

typedef unsigned char LkCoordByte;
typedef unsigned short int LkCoordWord;
typedef unsigned long LkCoordLong;
typedef unsigned __int64 LkCoordHuge;

//================================================================
// LkComp
//================================================================
// Each distribution (proposition) takes some values and
// this type defines restrictions on the possible number
// of gradatation for our distributions

typedef unsigned short int LkComp;
#define LkComp_Max   0xfffc // maximal value of distributions
#define LkComp_Min   0x0000 // minimal value of distributions
#define LkComp_Null  0xfffd // NULL value for distributions
#define LkComp_All   0xfffe // all values
#define LkComp_Any   0xffff // any value (unknown)

typedef unsigned char LkCompByte;
#define LkCompByte_Max  0xfc // maximal value of distributions
#define LkCompByte_Min  0x00 // minimal value of distributions
#define LkCompByte_Null 0xfd // NULL value for distributions
#define LkCompByte_All  0xfe // all values
#define LkCompByte_Any  0xff // any value (unknown)

typedef unsigned short int LkCompWord;
#define LkCompWord_Max   0xfffc // maximal value of distributions
#define LkCompWord_Min   0x0000 // minimal value of distributions
#define LkCompWord_Null  0xfffd // NULL value for distributions
#define LkCompWord_All   0xfffe // all values
#define LkCompWord_Any   0xffff // any value (unknown)

typedef unsigned long LkCompLong;
#define LkCompLong_Max   0x0ffffffc // maximal value of distributions
#define LkCompLong_Min   0x00000000 // minimal value of distributions
#define LkCompLong_Null  0xfffffffd // NULL value for distributions
#define LkCompLong_All   0xfffffffe // all values
#define LkCompLong_Any   0xffffffff // any value (unknown)

typedef unsigned __int64 LkCompHuge;
#define LkCompHuge_Max   0xfffffffffffffffc // maximal value of distributions
#define LkCompHuge_Min   0x0000000000000000 // minimal value of distributions
#define LkCompHuge_Null  0xfffffffffffffffd // NULL value for distributions
#define LkCompHuge_All   0xfffffffffffffffe // all values
#define LkCompHuge_Any   0xffffffffffffffff // any value (unknown)

typedef float LkCompFloat;
#define LkCompF_Max     1 // maximal value of distributions
#define LkCompF_Min    0 // minimal value of distributions
#define LkCompF_Null  -1 // NULL value for distributions
#define LkCompF_All   -2 // all values
#define LkCompF_Any   -3 // any value (unknown)

typedef double LkCompDouble;
#define LkCompD_Max     1 // maximal value of distributions
#define LkCompD_Min    0 // minimal value of distributions
#define LkCompD_Null  -1 // NULL value for distributions
#define LkCompD_All   -2 // all values
#define LkCompD_Any   -3 // any value (unknown)


//================================================================
// LkSect
//================================================================
// LkSect is a section consisting of only one segment

typedef unsigned long LkSect;
#define LkSect_Max 0xffffffff
#define LkSect_Min 0x00000000

typedef unsigned char LkSectByte;
#define LkSectByte_Max 0xff
#define LkSectByte_Min 0x00

typedef unsigned short int LkSectWord;
#define LkSectWord_Max 0xffff
#define LkSectWord_Min 0x0000

typedef unsigned long LkSectLong;
#define LkSectLong_Max 0xffffffff
#define LkSectLong_Min 0x00000000

typedef unsigned __int64 LkSectHuge;
#define LkSectHuge_Max 0xffffffffffffffff
#define LkSectHuge_Min 0x0000000000000000

//================================================================
// LkSegm
//================================================================
// Segments for making up sections, i.e., several segments
// are used to compose a section of necessisary length

typedef unsigned long LkSegm;
#define LkSect_Max 0xffffffff
#define LkSect_Min 0x00000000

typedef unsigned char LkSegmByte;
#define LkSegmByte_Max 0xff
#define LkSegmByte_Min 0x00

typedef unsigned short int LkSegmWord;
#define LkSegmWord_Max 0xffff
#define LkSegmWord_Min 0x0000

typedef unsigned long LkSegmLong;
#define LkSegmLong_Max 0xffffffff
#define LkSegmLong_Min 0x00000000

typedef unsigned __int64 LkSegmHuge;
#define LkSegmHuge_Max 0xffffffffffffffff
#define LkSegmHuge_Min 0x0000000000000000

//================================================================
// LkError
//================================================================
// Error codes returned by some functions

typedef unsigned long LkError; 
#define LkError_Success 0
#define LkError_General 1
#define LkError_Memory  2

//================================================================
// Class declarations
//================================================================

//
// LkVector 
//
template<class LkCompT> class LkVector;

//
// LkNode 
//
class LkNode;

//
// LkFuzzyCNF* 
//
template<class LkCompT> class LkFuzzyCNF;
template<class LkCompT> class LkFuzzyLogCNF;
template<class LkCompT> class LkFuzzyLoggCNF;

//
// LkCrispCNF* 
//
class LkCrispCNF; // One boolean component -- one byte
class LkCrispLogCNF; // Two boolean components -- two bytes
class LkCrispLoggCNF;

//
// LkSegmCNF* 
//
class LkSegmCNF; // Any number of boolean components
class LkSegmLogCNF; // Two boolean components -- one byte
class LkSegmLoggCNF;

//
// LkSectCNF* 
//
template<class LkSectT> class LkSectCNF;
template<class LkSectT> class LkSectLogCNF; // = LkSegmLogCNF
template<class LkSectT> class LkSectLoggCNF; // = LkSegmLoggCNF

//
// LkISTreeAssoc* 
//
template<class LkSectT> class LkISTreeAssoc;
template<class LkSectT> class LkISTreeAssocLog; 

//================================================================
// Global declarations
//================================================================

#include "LkVector.h"
#include "LkNode.h"
#include "LkFuzzyCNF.h"
#include "LkCrispCNF.h"
#include "LkSegmCNF.h"
#include "LkISTreeAssocLog.h"
