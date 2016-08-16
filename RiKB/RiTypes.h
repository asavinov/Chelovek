//================================================================
// RiCoord
//================================================================
// Each variable takes some number of values. In addition, 
// there is some number of variables themselves. Generally, 
// the number of either variables or values is reffered to 
// as coordinate. Coordinate is the number of children for 
// a parent. For example, if the parent is some variable,
// then each its value has some number and hence coordinate.

// This type defines restrictions on the possible number of
// both variables and values of variables

typedef unsigned short int RiCoord;
#define RiCoord_Max   0xfff0 // maximal value of variables
#define RiCoord_Min   0x0000 // minimal value of variables
#define RiCoord_Null  0xfff0 + 1 // NULL value for coordinates
#define RiCoord_All   0xfff0 + 2 // all coordinates
#define RiCoord_Any   0xfff0 + 3 // Any coordinate (unknown)
#define RiCoord_MinusInf 0xfff0 + 4 // Minus infinity
#define RiCoord_PlusInf  0xfff0 + 5 // Plus infinity
#define RiCoord_Inf      0xfff0 + 6 // Infinity

typedef unsigned char RiCoord1;
typedef unsigned short int RiCoord2;
typedef unsigned long RiCoord4;


//================================================================
// RiOffs
//================================================================
// Offset in multi-dimensional space (global coordinate)
typedef unsigned long RiOffs;
#define RiOffs_Max   0xfffffff0 // maximal value of offsets
#define RiOffs_Min   0x00000000 // minimal value of offsets
#define RiOffs_Null  0xfffffff0 + 1 // NULL value for offsets
#define RiOffs_All   0xfffffff0 + 2 // all offsets
#define RiOffs_Any   0xfffffff0 + 3 // any offsets (unknown)


//================================================================
// RiLongOffs
//================================================================
// Offset in multi-dimensional space (global coordinate)
typedef unsigned __int64 RiLongOffs;


//================================================================
// RiComp
//================================================================
// Each distribution (proposition) takes some values and
// this type defines restrictions on the possible number
// of gradatation for our distributions

typedef unsigned short int RiComp;
#define RiComp_Max   0xfffc // maximal value of distributions
#define RiComp_Min   0x0000 // minimal value of distributions
#define RiComp_Null  0xfffd // NULL value for distributions
#define RiComp_All   0xfffe // all values
#define RiComp_Any   0xffff // any value (unknown)

typedef unsigned char RiComp1;
#define RiComp1_Max  0xfc // maximal value of distributions
#define RiComp1_Min  0x00 // minimal value of distributions
#define RiComp1_Null 0xfd // NULL value for distributions
#define RiComp1_All  0xfe // all values
#define RiComp1_Any  0xff // any value (unknown)

typedef unsigned short int RiComp2;
#define RiComp2_Max   0xfffc // maximal value of distributions
#define RiComp2_Min   0x0000 // minimal value of distributions
#define RiComp2_Null  0xfffd // NULL value for distributions
#define RiComp2_All   0xfffe // all values
#define RiComp2_Any   0xffff // any value (unknown)

typedef unsigned long RiComp4;
#define RiComp4_Max   0x0ffffffc // maximal value of distributions
#define RiComp4_Min   0x00000000 // minimal value of distributions
#define RiComp4_Null  0xfffffffd // NULL value for distributions
#define RiComp4_All   0xfffffffe // all values
#define RiComp4_Any   0xffffffff // any value (unknown)

typedef float RiCompF;
#define RiCompFMax     1 // maximal value of distributions
#define RiCompF_Min    0 // minimal value of distributions
#define RiCompF_Null  -1 // NULL value for distributions
#define RiCompF_All   -2 // all values
#define RiCompF_Any   -3 // any value (unknown)

typedef double RiCompD;
#define RiCompFMax     1 // maximal value of distributions
#define RiCompF_Min    0 // minimal value of distributions
#define RiCompF_Null  -1 // NULL value for distributions
#define RiCompF_All   -2 // all values
#define RiCompF_Any   -3 // any value (unknown)


//================================================================
// RiSect
//================================================================
// 

typedef unsigned long RiSect;
#define RiSect_Max 0xffffffff
#define RiSect_Min 0x00000000

typedef unsigned char RiSect1;
typedef unsigned short int RiSect2;
typedef unsigned long RiSect4;
typedef unsigned __int64 RiSect8;

//================================================================
// RiOperation
//================================================================
// Children of parents can be combined with some operation
// (logical connective), i.e., each parent is assigned an
// operation which is used for combining children. For
// example, variables in multi-dimensional space are 
// combined with AND (Cartesian product), while values
// of one variable are combined with OR.

typedef unsigned char RiOperation;
#define RiOperation_Null  0
#define RiOperation_Or    1
#define RiOperation_And   2


//================================================================
// RiVarType
//================================================================

typedef unsigned char RiVarType;
#define RiVarType_Null     0
#define RiVarType_Any      1

//================================================================
// RiValType
//================================================================

typedef unsigned char RiValType;
#define RiValType_Null     0
#define RiValType_Any      1

//================================================================
// RiSVHeader
//================================================================
// Header of sectioned vector with flags

typedef struct {

  unsigned short int Flags;

  RiOperation Operation; // OR for disjunctions, AND for conjunctions

  // Aggregate (global) characteristics
  RiCoord Rank; // The number of non-trivial sections (depends on interpretation)
  RiComp Min;
  RiComp Max;
  RiComp4 Freq; // Support, i.e., the number of data points (no weights)

  // Parameters of generation
  RiCoord OldVar; // Last modified (generalized) variable (section)
  RiComp  OldComp; // The value of the previous component changed
  RiCoord OldCompOffs; // Offset in bytes to the generalized component

  } RiSVHeader;
#define RiSVFlags_Absorbed     0x0001 // Absent vector
#define RiSVFlags_Locked       0x0002 // Locked by someone
#define RiSVFlags_MinUp        0x0004 // The disjunctions constant has increased since last modification
#define RiSVFlags_MaxDown      0x0008 // The conjunction constant has decreased since last modification
#define RiSVFlags_IsGoalVar    0x0010 // There is a component in goal section


//================================================================
// RiLine
//================================================================

typedef unsigned short int RiLine;
#define RiLine_Max   0xfffc // maximal value of distributions
#define RiLine_Min   0x0000 // minimal value of distributions
#define RiLine_Null  0xfffd // NULL value for distributions
#define RiLine_All   0xfffe // all values
#define RiLine_Any   0xffff // any value (unknown)

//================================================================
// RiError
//================================================================
// Error codes returned by some functions

typedef unsigned long RiError; 
#define RiError_Success 0
#define RiError_General 1
#define RiError_Memory  2

//================================================================
// RiProcess
//================================================================
// Process codes used to identify process states of objects

typedef unsigned char RiProcess; 
#define RiProcess_Null         0
#define RiProcess_FindPatterns 1
#define RiProcess_FillPatterns 2

//================================================================
// RiOptAnalysis
//================================================================

typedef struct {

  RiCoord MaxValues; // Maximal number of values to detect

  } RiOptAnalysis;

//================================================================
// RiOptInduction
//================================================================

typedef struct {

  RiLine MaxNewLines; // How more lines can we add before deleting (buffer)

  //
  // Limitations (depend on the whole set of patterns)
  //
  RiLine MaxLines; // Do not generate more patterns

  //
  // Check when generated (depend on the only pattern)
  //
  RiCoord MaxRank; // Do not generate and evaluate patterns with higher rank
  RiCoord GoalVar; // Generate only patterns involving this variable

  //
  // Options of ordering 
  //

  } RiOptInduction;

//================================================================
// Class declarations
//================================================================

class RiNode;

class RiSpace;
class RiVariable;
class RiValue;

class RiKB;
class RiRules;

class RiCNF;

class RiCNF_F;

class RiCNF_B;

//================================================================
// Global declarations
//================================================================

RiCoord PointerArrayLength( RiNode** ArrayOfPointers );
RiCoord Pointer2Index( RiNode** ArrayOfPointers, RiNode* Pointer );

long Offset2Coordinates( long* AP_Array, long Offset, RiCoord* Coordinates );
long Coordinates2Offset( long* AP_Array, RiCoord* Coordinates );

void Output( int Percent, long Param );

