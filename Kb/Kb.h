//================================================================
// KbError
//================================================================
// Error codes returned by some functions

typedef unsigned long KbError; 
#define KbError_Success            0
#define KbError_General            1
#define KbError_Memory             2
#define KbError_EOF                3 // End of file
#define KbError_EOB                4 // End of buffer
#define KbError_EOL                5 // End of line/record
#define KbError_OpenFile           6 // Cannot open file
#define KbError_BadIndex           7 // Bad element
#define KbError_BadSize            8 // Bad size

//================================================================
// Class declarations
//================================================================

class KbKB;

class KbField;
class KbDataSourceCSV;

//================================================================
// Global declarations
//================================================================

#include "..\RiKB\Ri.h" // !!!!!!!!!! To be deleted !!!!!!!!!!

#include "KbKB.h"

#include "KbDataSourceCSV.h"
