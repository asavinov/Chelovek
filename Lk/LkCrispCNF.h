#if !defined(__LkCrispCNF_h__)
#define __LkCrispCNF_h__

/*
Syntax describes variables and values and is intended for 
fast accessing components in vectors and other structures. 
Generally, all information that depends on only variable 
(section) number can be put here.
*/

class LkCrispSyn 
{
public:
	LkCoord m_Size; // Number of comps in section (values of variable)
	LkCoord m_AccuSum; // Accumulated sum of values of variables 
	LkCoordHuge m_AccuProd; // Accumulated product of values of variables 

	LkCoord m_PointCoord; // Coordinate (value) of the current point to be added to the matrix  (from the section start)
	LkCoord m_PointOffset; // Accumulated (from the line start)

	// The same in bytes (offsets or lengths). Corresponding value multiplied on sizeof(LkCompByte)
	LkCoord m_SizeB; // Length of the section in bytes 
	LkCoord m_AccuSumB; // Offset to the section 
	LkCoordHuge m_AccuProdB; // Offset to the point in multidim space 

	LkCoord m_PointCoordB; // Offset to the component corresponding to the current point (from the section start)
	LkCoord m_PointOffsetB; // Offset to the component in bytes (from the line start)

};

/*
Section header 
*/
class LkCrispSect 
{
public:
	LkCompByte *m_Comps; // Pointer to the array of all section comps 
	LkCoord m_Length; // Rank -- number of non-empty comps 
	LkCompLong m_CompSum; // Sum of components
};

/*
Line header 
*/
class LkCrispLine 
{
public:
	LkCompByte *m_Comps; // Pointer to the array of all line comps 

	LkCrispSect *m_Sects; // Pointer to the array of section headers 

	LkCoord m_Length; // Rank on variables -- number of non-empty sections 
	LkCoord m_TotLength; // Rank on components -- number of non-empty comps in all sects 

	LkCompByte m_Const; // Constant of the vector 
	LkCompLong m_CompSum; // Sum of components
	LkCompFloat m_Info; // Quantity of information

	unsigned char m_Flags;

	// Parameters of generation
	LkCoord m_OldVar; // Last modified (induced) variable (section)
	LkCompByte m_OldComp; // The value of the last comp changed (induced) 
	LkCoord m_OldCompOffs; // Offset in comps to the modified (induced) comp

	// Main lists (orders)
	LkCrispLine *m_InfoNext; // Info list 
	LkCrispLine *m_InfoPrev;
	LkCrispLine *m_CompSumNext; // CompSum (absorb) list 
	LkCrispLine *m_CompSumPrev;

	// Stable/Old/New sublists 
	LkCrispLine *m_SubNext; 
	LkCrispLine *m_SubPrev;

	unsigned char m_SubListType; // No/Stab/Old/New/Free
};

#define LkCrispLine_Absorbed       0x01 // Absent vector
#define LkCrispLine_Locked         0x02 // Locked by someone
#define LkCrispLine_ConstUp        0x04 // Constant has increased since last modification
#define LkCrispLine_ConstDown      0x08 // Constant has decreased since last modification
#define LkCrispLine_IsGoalVar      0x10 // Goal section is not empty

#define LkCrispLine_NoList         0
#define LkCrispLine_StabList       1
#define LkCrispLine_OldList        2
#define LkCrispLine_NewList        3
#define LkCrispLine_FreeList       4

/*
Crisp CNF
This class is intended mainly to carry out difficult 
transformation from DNF to CNF. Its main operation is 
AddPoint() which rewrites all lines of the matrix.
*/
class LkCrispCNF 
{
private:
	LkCrispSyn *m_Syn; // Pointer to the array of syntax information 

	LkCoord m_Width; // Number of variables (width of the matrix)
	LkCoord m_LineWidth; // Total number of components in one line 
	LkCoord m_LineWidthB; // Line width in bytes =m_LineWidth*sizeof(LkCompByte)

	LkCompByte m_Min;
	LkCompByte m_Max;

	LkCoord m_NumLines; // Number of non-empty lines (real current length of the matrix)

	// Options/Limitations
	LkCoord m_MaxLines; // Maximal number of lines (buffer size) 
	LkCoord m_GoalVar; // Goal variable 
	LkCoord m_MaxRank; // Maximal rank on variables for all lines 
	LkCoord m_MaxTotRank; // Maximal rank on components for all lines
	LkCompByte m_MaxConst; // Maximal constant for all lines

	LkCompByte *m_Comps; // Array of all matrix components (semantics)

	LkCrispLine *m_Lines; // Array of line headers (m_Size elements)

	LkCrispSect *m_Sects; // Array of section headers (m_Size*m_Width elements) 

	// Access to the lists and sublists
	LkCrispLine *m_InfoListFirst; // Info ordered list (to remove lines)
	LkCrispLine *m_InfoListLast;

	LkCrispLine *m_CompSumListFirst; // CompSum ordered list (to absorb lines)
	LkCrispLine *m_CompSumListLast;

	LkCrispLine *m_StabListFirst; // Sublist of stable lines (unmodified)
	LkCrispLine *m_StabListLast;

	LkCrispLine *m_OldListFirst; // Sublist of old lines (to be processed)
	LkCrispLine *m_OldListLast;

	LkCrispLine *m_NewListFirst; // Sublist of new lines (generated)
	LkCrispLine *m_NewListLast;

	LkCrispLine *m_FreeListFirst; // Sublist of free lines (not used)
	LkCrispLine *m_FreeListLast;

	// Diagnostics/statistics
#ifdef _DIAG
	long m_DiagAbsorbByStab;
	long m_DiagAbsorbByStab_Const;
	long m_DiagAbsorbByStab_Critical;
	long m_DiagAbsorbByStab_Sects;
	long m_DiagAbsorbByStab_Comps;

	long m_DiagAbsorbByNew;
#endif // _DIAG

public:

	//
	// Construction
	//
	LkCrispCNF( LkVector<LkCoord> &Sizes, LkCoord MaxLines, LkCoord GoalVar = LkCoord_Null, LkCoord MaxRank = LkCoord_Null, LkCoord MaxTotRank = LkCoord_Null );

	//
	// Destruction
	//
	~LkCrispCNF();

	//
	// Induction functions 
	//
	LkError ProcessPoint( LkVector<LkCoord> &Point );

	//
	// Obtaining result of induction 
	//

protected:

	inline void InfoListAppend( LkCrispLine *Line );
	inline void CompSumListAppend( LkCrispLine *Line );
	inline void StabListAppend( LkCrispLine *Line );
	inline void OldListAppend( LkCrispLine *Line );
	inline void NewListAppend( LkCrispLine *Line );
	inline void FreeListAppend( LkCrispLine *Line );

	inline void InfoListDelete( LkCrispLine *Line );
	inline void CompSumListDelete( LkCrispLine *Line );
	inline void StabListDelete( LkCrispLine *Line );
	inline void OldListDelete( LkCrispLine *Line );
	inline void NewListDelete( LkCrispLine *Line );
	inline void FreeListDelete( LkCrispLine *Line );
	inline void SubListDelete( LkCrispLine *Line );

	inline void InfoListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void CompSumListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void StabListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void OldListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void NewListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void FreeListInsert( LkCrispLine *Line, LkCrispLine *Line1 );
	inline void SubListInsert( LkCrispLine *Line, LkCrispLine *Line1 );

	inline LkCrispLine* InfoListFind( LkCrispLine *Line, LkCrispLine *Line1 );
	inline LkCrispLine* CompSumListFind( LkCrispLine *Line, LkCrispLine *Line1 );
	inline LkCrispLine* NewListFind( LkCrispLine *Line );

	inline BOOL InfoListIsOut( LkCompFloat LineInfo, LkCompLong LineCompSum );

	inline void CopyLine( LkCrispLine *LineSrc, LkCrispLine *LineDst ); // Copy content of one line into another
	inline void ReduceLine( LkCrispLine *Line, LkCompByte Constant ); // Reduce line to new constant and recalculate parameters

	inline BOOL IsAbsorbedByStab( LkCrispLine *Line );
	inline BOOL IsAbsorbedByNew( LkCrispLine *Line );
	inline BOOL IsLineAbsorbedNew( LkCrispLine *Line1, LkCrispLine *Line2 );
};

#endif // !defined(__LkCrispCNF_h__)
