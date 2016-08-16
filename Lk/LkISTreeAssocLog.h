#if !defined(__LkISTreeAssocLog_h__)
#define __LkISTreeAssocLog_h__

#define HDONLY    0x80000000L   /* flag for head only item in path */

/*
Itemset tree node. It does not have a constructor/destructor and 
is always created/destroyed by means of special member-functions.
*/

template <class LkCompT> 
class LkISTreeAssocLogNode 
{
public:
	LkISTreeAssocLogNode<LkCompT> *m_Parent; // Parent node (on the previous level)
	LkISTreeAssocLogNode<LkCompT> *m_Successor; // Successor node (on the same level)

	LkCoord m_Item; // preceeding item number (coordinate, index in parent) in the itemset corresponding to this node

	LkCoord m_DataStart; // Offset of data vector 
	LkCoord m_DataSize; // Size of data vector 

	LkCoord m_ChildSize; // Size of child vector 

	LkCompT m_Data[1]; // Data vector
	// LkCompT m_Data[2];
	// ---/---/---
	// LkCompT m_Data[m_DataSize];

	// Then child pointer array follow consisting of m_ChildSize pointers

private:

	LkCoord Item() { return (m_Item & ~HDONLY); }
	LkCoord IsItemHDOnly { return (m_Item & HDONLY); }

protected:

};

/*
Itemset tree for the case of logical variables with cumulative 
distribution (accumulation of frequencies). 
This class is intended mainly to calculate the itemset 
characteristics such frequencies, correlations etc. 
Its main operation is GenerateCandidates() which generates 
all canditate itemsets given the current state of the tree. 
GenerateRules() is intended for finding rules given the hash 
tree with frequences.
*/

template <class LkCompT> 
class LkISTreeAssocLog 
{
private:
	LkVector<LkISTreeAssocLogNode<LkCompT>*> m_Levels; // Array of pointers to the first node of each level 

	LkCoord m_NumItemset; // Number of itemsets in the tree (number of sets counted)

	LkCoord m_Width; // Number of variables 

	LkCompT m_Min; // Current minimal frequency in the tree
	LkCompT m_Max; // Current maximal frequency in the tree

	// Options/Limitations
	double m_MinSupport; // Minimal support of an item set
	double m_Confidence; // Minimal confidence of a rule

	LkCoord m_GoalVar; // Goal variable 
	LkCoord m_MaxRank; // Maximal rank on variables for all lines (max level)
/*
	int arem; /* additional rule evaluation measure */
	double minval; /* minimal evaluation measure value */
*/

/*
	// Working variables
	ISNODE         *curr;         /* current node in tree */
	ISNODE         *crex;         /* current node for rule extraction */
	long           index;         /* index in current node */
	long           rlen;          /* current rule length */
	long           head;          /* item in head of previous rule */
	long           *path;         /* path buffer */
	int            hdonly;        /* head only item in current set */
	char           apps[1];       /* item appearances */
*/

	// Diagnostics/statistics
#ifdef _DIAG
	long m_DiagAbsorbByNew;
#endif // _DIAG

public:

	//
	// Construction
	//
	LkISTreeAssocLog( LkCoord NumVar, LkCoord GoalVar = LkCoord_Null, LkCoord MaxRank = LkCoord_Null );

	//
	// Destruction
	//
	~LkISTreeAssocLog();

	//
	// Procession functions 
	//
	LkError GenerateCandidates( );
	LkError GenerateRules( );

	//
	// Obtaining result of induction 
	//

protected:

};

#endif // !defined(__LkISTreeAssocLog_h__)
