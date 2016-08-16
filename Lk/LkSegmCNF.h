#if !defined(__LkSegmCNF_h__)
#define __LkSegmCNF_h__

/*
Syntax describes variables and values and is intended for 
fast accessing components in vectors and other structures. 
Generally, all information that depends on only variable 
(section) number can be put here.
*/

class LkSegmSyn 
{
public:
	LkCoord m_Size; // Number of comps in section (values of variable)
	LkCoord m_AccuSum; // Accumulated sum of values of variables 
	LkCoordHuge m_AccuProd; // Accumulated product of values of variables 

	LkCoord m_PointCoord; // Coordinate (value) of the current point to be added to the matrix  (from the section start)
	LkCoord m_PointOffset; // Accumulated (from the line start)

	// The same in bytes (offsets or lengths). Corresponding value multiplied on sizeof(LkCompByte)
	LkCoord m_SizeB; // Length of the section in segments 
	LkCoord m_AccuSumB; // Offset to the section in segments 
	LkCoordHuge m_AccuProdB; // Offset to the point in multidim space in segments 

	LkCoord m_PointCoordB; // Offset in segments to the segment corresponding to the current point (from the section start)
	LkCoord m_PointOffsetB; // Offset in segments to the segment corresponding to the current point (from the line start)

	LkSegm m_PointSegm; // Point segment with one (corresponding to the point) bit set to 1
};

/*
Section header 
*/
class LkSegmSect 
{
public:
	LkSegm *m_Comps; // Pointer to the array of all section comps 
	LkCoord m_Length; // Rank -- number of non-empty comps 
	LkCompLong m_CompSum; // Sum of components
};

/*
Line header 
*/
class LkSegmLine 
{
public:
	LkSegm *m_Comps; // Pointer to the array of all line comps 

	LkSegmSect *m_Sects; // Pointer to the array of section headers 

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
	LkSegmLine *m_InfoNext; // Info list 
	LkSegmLine *m_InfoPrev;
	LkSegmLine *m_CompSumNext; // CompSum (absorb) list 
	LkSegmLine *m_CompSumPrev;

	// Stable/Old/New sublists 
	LkSegmLine *m_SubNext; 
	LkSegmLine *m_SubPrev;

	unsigned char m_SubListType; // No/Stab/Old/New/Free
};

#define LkSegmLine_Absorbed       0x01 // Absent vector
#define LkSegmLine_Locked         0x02 // Locked by someone
#define LkSegmLine_ConstUp        0x04 // Constant has increased since last modification
#define LkSegmLine_ConstDown      0x08 // Constant has decreased since last modification
#define LkSegmLine_IsGoalVar      0x10 // Goal section is not empty

#define LkSegmLine_NoList         0
#define LkSegmLine_StabList       1
#define LkSegmLine_OldList        2
#define LkSegmLine_NewList        3
#define LkSegmLine_FreeList       4

/*
Segm CNF
This class is intended mainly to carry out difficult 
transformation from DNF to CNF. Its main operation is 
AddPoint() which rewrites all lines of the matrix.
*/
class LkSegmCNF 
{
private:
	LkSegmSyn *m_Syn; // Pointer to the array of syntax information 

	LkCoord m_Width; // Number of variables (width of the matrix)
	LkCoord m_LineWidth; // Total number of components in one line 
	LkCoord m_LineWidthB; // Line width in segments 

	LkCompByte m_Min;
	LkCompByte m_Max;

	LkCoord m_NumLines; // Number of non-empty lines (real current length of the matrix)

	LkCoord m_MaxLines; // Maximal number of lines (buffer size) 
	LkCoord m_GoalVar; // Goal variable 
	LkCoord m_MaxRank; // Maximal rank on variables for all lines 
	LkCoord m_MaxTotRank; // Maximal rank on components for all lines
	LkCompByte m_MaxConst; // Maximal constant for all lines

	LkSegm *m_Comps; // Array of all matrix components (semantics)

	LkSegmLine *m_Lines; // Array of line headers (m_Size elements)

	LkSegmSect *m_Sects; // Array of section headers (m_Size*m_Width elements) 

	// Access to the lists and sublists
	LkSegmLine *m_InfoListFirst; // Info ordered list (to remove lines)
	LkSegmLine *m_InfoListLast;

	LkSegmLine *m_CompSumListFirst; // CompSum ordered list (to absorb lines)
	LkSegmLine *m_CompSumListLast;

	LkSegmLine *m_StabListFirst; // Sublist of stable lines (unmodified)
	LkSegmLine *m_StabListLast;

	LkSegmLine *m_OldListFirst; // Sublist of old lines (to be processed)
	LkSegmLine *m_OldListLast;

	LkSegmLine *m_NewListFirst; // Sublist of new lines (generated)
	LkSegmLine *m_NewListLast;

	LkSegmLine *m_FreeListFirst; // Sublist of free lines (not used)
	LkSegmLine *m_FreeListLast;

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
	LkSegmCNF( LkVector<LkCoord> &Sizes, LkCoord MaxLines, LkCoord GoalVar = LkCoord_Null, LkCoord MaxRank = LkCoord_Null, LkCoord MaxTotRank = LkCoord_Null );

	//
	// Destruction
	//
	~LkSegmCNF();

	//
	// Induction functions 
	//
	LkError ProcessPoint( LkVector<LkCoord> &Point );

	//
	// Obtaining result of induction 
	//

protected:

	inline void InfoListAppend( LkSegmLine *Line );
	inline void CompSumListAppend( LkSegmLine *Line );
	inline void StabListAppend( LkSegmLine *Line );
	inline void OldListAppend( LkSegmLine *Line );
	inline void NewListAppend( LkSegmLine *Line );
	inline void FreeListAppend( LkSegmLine *Line );

	inline void InfoListDelete( LkSegmLine *Line );
	inline void CompSumListDelete( LkSegmLine *Line );
	inline void StabListDelete( LkSegmLine *Line );
	inline void OldListDelete( LkSegmLine *Line );
	inline void NewListDelete( LkSegmLine *Line );
	inline void FreeListDelete( LkSegmLine *Line );
	inline void SubListDelete( LkSegmLine *Line );

	inline void InfoListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void CompSumListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void StabListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void OldListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void NewListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void FreeListInsert( LkSegmLine *Line, LkSegmLine *Line1 );
	inline void SubListInsert( LkSegmLine *Line, LkSegmLine *Line1 );

	inline LkSegmLine* InfoListFind( LkSegmLine *Line, LkSegmLine *Line1 );
	inline LkSegmLine* CompSumListFind( LkSegmLine *Line, LkSegmLine *Line1 );
	inline LkSegmLine* NewListFind( LkSegmLine *Line );

	inline BOOL InfoListIsOut( LkCompFloat LineInfo, LkCompLong LineCompSum );

	inline void CopyLine( LkSegmLine *LineSrc, LkSegmLine *LineDst ); // Copy content of one line into another
	inline void ReduceLine( LkSegmLine *Line, LkCompByte Constant ); // Reduce line to new constant and recalculate parameters

	inline BOOL IsAbsorbedByStab( LkSegmLine *Line );
	inline BOOL IsAbsorbedByNew( LkSegmLine *Line );
	inline BOOL IsLineAbsorbedNew( LkSegmLine *Line1, LkSegmLine *Line2 );
};

#endif // !defined(__LkSegmCNF_h__)
