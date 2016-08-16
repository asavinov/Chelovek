#if !defined(__LkFuzzyCNF_h__)
#define __LkFuzzyCNF_h__

/*
Syntax describes variables and values and is intended for 
fast accessing components in vectors and other structures. 
Generally, all information that depends on only variable 
(section) number can be put here.
*/

class LkFuzzySyn 
{
public:
	LkCoord m_Size; // Number of comps in section (values of variable)
	LkCoord m_AccuSum; // Accumulated sum of values of variables 
	LkCoordHuge m_AccuProd; // Accumulated product of values of variables 

	LkCoord m_PointCoord; // Coordinate (value) of the current point to be added to the matrix  (from the section start)
	LkCoord m_PointOffset; // Accumulated (from the line start)

	// The same in bytes (offsets or lengths). Corresponding value multiplied on sizeof(LkCompT)
	LkCoord m_SizeB; // Length of the section in bytes 
	LkCoord m_AccuSumB; // Offset to the section 
	LkCoordHuge m_AccuProdB; // Offset to the point in multidim space 

	LkCoord m_PointCoordB; // Offset to the component corresponding to the current point (from the section start)
	LkCoord m_PointOffsetB; // Offset to the component in bytes (from the line start)

};

/*
Section header 
*/
template <class LkCompT> 
class LkFuzzySect 
{
public:
	LkCompT *m_Comps; // Pointer to the array of all section comps 
	LkCoord m_Length; // Rank -- number of non-empty comps 
	LkCompLong m_CompSum; // Sum of components
};

/*
Line header 
*/

template <class LkCompT> 
class LkFuzzyLine 
{
public:
	LkCompT *m_Comps; // Pointer to the array of all line comps 

	LkFuzzySect<LkCompT> *m_Sects; // Pointer to the array of section headers 

	LkCoord m_Length; // Rank on variables -- number of non-empty sections 
	LkCoord m_TotLength; // Rank on components -- number of non-empty comps in all sects 

	LkCompT m_Const; // Constant of the vector 
	LkCompLong m_CompSum; // Sum of components
	LkCompFloat m_Info; // Quantity of information

	unsigned char m_Flags;

	// Parameters of generation
	LkCoord m_OldVar; // Last modified (induced) variable (section)
	LkCompT m_OldComp; // The value of the last comp changed (induced) 
	LkCoord m_OldCompOffs; // Offset in comps to the modified (induced) comp

	// Main lists (orders)
	LkFuzzyLine<LkCompT> *m_InfoNext; // Info list 
	LkFuzzyLine<LkCompT> *m_InfoPrev;
	LkFuzzyLine<LkCompT> *m_CompSumNext; // CompSum (absorb) list 
	LkFuzzyLine<LkCompT> *m_CompSumPrev;

	// Stable/Old/New sublists 
	LkFuzzyLine<LkCompT> *m_SubNext; 
	LkFuzzyLine<LkCompT> *m_SubPrev;

	unsigned char m_SubListType; // No/Stab/Old/New/Free
};

#define LkFuzzyLine_Absorbed       0x01 // Absent vector
#define LkFuzzyLine_Locked         0x02 // Locked by someone
#define LkFuzzyLine_ConstUp        0x04 // Constant has increased since last modification
#define LkFuzzyLine_ConstDown      0x08 // Constant has decreased since last modification
#define LkFuzzyLine_IsGoalVar      0x10 // Goal section is not empty

#define LkFuzzyLine_NoList         0
#define LkFuzzyLine_StabList       1
#define LkFuzzyLine_OldList        2
#define LkFuzzyLine_NewList        3
#define LkFuzzyLine_FreeList       4

/*
Fuzzy CNF
This class is intended mainly to carry out difficult 
transformation from DNF to CNF. Its main operation is 
AddPoint() which rewrites all lines of the matrix.
*/
template <class LkCompT> 
class LkFuzzyCNF 
{
private:
	LkFuzzySyn *m_Syn; // Pointer to the array of syntax information 

	LkCoord m_Width; // Number of variables (width of the matrix)
	LkCoord m_LineWidth; // Total number of components in one line 
	LkCoord m_LineWidthB; // Line width in bytes =m_LineWidth*sizeof(LkCompT)

	LkCompT m_Min;
	LkCompT m_Max;

	LkCoord m_NumLines; // Number of non-empty lines (real current length of the matrix)

	// Options/Limitations
	LkCoord m_MaxLines; // Maximal number of lines (buffer size) 
	LkCoord m_GoalVar; // Goal variable 
	LkCoord m_MaxRank; // Maximal rank on variables for all lines 
	LkCoord m_MaxTotRank; // Maximal rank on components for all lines
	LkCompT m_MaxConst; // Maximal constant for all lines

	LkCompT *m_Comps; // Array of all matrix components (semantics)

	LkFuzzyLine<LkCompT> *m_Lines; // Array of line headers (m_Size elements)

	LkFuzzySect<LkCompT> *m_Sects; // Array of section headers (m_Size*m_Width elements) 

	// Access to the lists and sublists
	LkFuzzyLine<LkCompT> *m_InfoListFirst; // Info ordered list (to remove lines)
	LkFuzzyLine<LkCompT> *m_InfoListLast;

	LkFuzzyLine<LkCompT> *m_CompSumListFirst; // CompSum ordered list (to absorb lines)
	LkFuzzyLine<LkCompT> *m_CompSumListLast;

	LkFuzzyLine<LkCompT> *m_StabListFirst; // Sublist of stable lines (unmodified)
	LkFuzzyLine<LkCompT> *m_StabListLast;

	LkFuzzyLine<LkCompT> *m_OldListFirst; // Sublist of old lines (to be processed)
	LkFuzzyLine<LkCompT> *m_OldListLast;

	LkFuzzyLine<LkCompT> *m_NewListFirst; // Sublist of new lines (generated)
	LkFuzzyLine<LkCompT> *m_NewListLast;

	LkFuzzyLine<LkCompT> *m_FreeListFirst; // Sublist of free lines (not used)
	LkFuzzyLine<LkCompT> *m_FreeListLast;

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
	LkFuzzyCNF( LkVector<LkCoord> &Sizes, LkCoord MaxLines, LkCoord GoalVar = LkCoord_Null, LkCoord MaxRank = LkCoord_Null, LkCoord MaxTotRank = LkCoord_Null, LkCompT MaxConst = 0 );

	//
	// Destruction
	//
	~LkFuzzyCNF();

	//
	// Induction functions 
	//
	LkError ProcessPoint( LkVector<LkCoord> &Point, LkCompT CompInPoint );

	//
	// Obtaining result of induction 
	//

protected:

	inline void InfoListAppend( LkFuzzyLine<LkCompT> *Line );
	inline void CompSumListAppend( LkFuzzyLine<LkCompT> *Line );
	inline void StabListAppend( LkFuzzyLine<LkCompT> *Line );
	inline void OldListAppend( LkFuzzyLine<LkCompT> *Line );
	inline void NewListAppend( LkFuzzyLine<LkCompT> *Line );
	inline void FreeListAppend( LkFuzzyLine<LkCompT> *Line );

	inline void InfoListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void CompSumListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void StabListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void OldListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void NewListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void FreeListDelete( LkFuzzyLine<LkCompT> *Line );
	inline void SubListDelete( LkFuzzyLine<LkCompT> *Line );

	inline void InfoListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void CompSumListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void StabListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void OldListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void NewListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void FreeListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline void SubListInsert( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );

	inline LkFuzzyLine<LkCompT>* InfoListFind( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline LkFuzzyLine<LkCompT>* CompSumListFind( LkFuzzyLine<LkCompT> *Line, LkFuzzyLine<LkCompT> *Line1 );
	inline LkFuzzyLine<LkCompT>* NewListFind( LkFuzzyLine<LkCompT> *Line );

	inline BOOL InfoListIsOut( LkCompFloat LineInfo, LkCompLong LineCompSum );

	inline void CopyLine( LkFuzzyLine<LkCompT> *LineSrc, LkFuzzyLine<LkCompT> *LineDst ); // Copy content of one line into another
	inline void ReduceLine( LkFuzzyLine<LkCompT> *Line, LkCompT Constant ); // Reduce line to new constant and recalculate parameters

	inline BOOL IsAbsorbedByStab( LkFuzzyLine<LkCompT> *Line );
	inline BOOL IsAbsorbedByNew( LkFuzzyLine<LkCompT> *Line );
	inline BOOL IsLineAbsorbedNew( LkFuzzyLine<LkCompT> *Line1, LkFuzzyLine<LkCompT> *Line2 );
};

void f();

#endif // !defined(__LkFuzzyCNF_h__)
