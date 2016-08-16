#if !defined(__LkVectorVector_h__)
#define __LkVectorVector_h__

/*
This is a vector of vectors. It is primarily designed as a 
sectioned vector data structure. Thus each vector in this 
class represents one section. 

The main use of this class is accessing components given 
their variable (vector number) and value (comp number) as 
well as aggregate characteristics such as Length.

In contrast to ordinary vector this class does not have 
m_EmptyComp member, but it does have a member m_TotLength 
with the total length of all sections. Generally all members 
with Tot prefix are meant as some totals of all sections.
*/

template <class LkCompT> 
class LkVectorVector
{
private:

	LkVector<LkCompT> *m_Data; // Pointer to the array of vectors (sections)
	LkCoord m_Size; // Number of vectors (sections)

	LkCoord m_Length; // Number of non-empty vectors (sections)

	LkCoord m_GrowBy; // Number of vectors added when expanding the number of vectors 

	LkCoord m_TotLength; // Total number of non-empty comps in all sects 

public:

	//
	// Construction
	//
	LkVectorVector(); // Default constructor
	LkVectorVector( LkCoord Size, LkCoord GrowBy = 1  ); // Empty template with known number of sections only
	LkVectorVector( LkVector<LkCompT> &Sizes, LkCompT EmptyComp = 0, LkCoord GrowBy = 1 ); // Parameters describe the sections

	//
	// Destruction
	//
	~LkVectorVector();

	//
	// General functions
	//
	LkCoord GetSize() const; // N: Number of sections 
	LkCoord SetSize( LkCoord Size, LkCoord GrowBy = 1 ); // Set new number of sections (reallocation only when necessary)

	LkCoord GetLength() const; // Returns the number of non-empty comps in vector
	void SetLength( LkCoord Length ); // Sets new (correct!) number of non-empty comps
	void IncrLength(); // Increments the length field (usually after changing one components to non-empty value from empty)
	void DecrLength(); // Decrements the length field (usually after changing one components to empty value from non-empty)
	LkCoord UpdateLength(); // Recalculates the length field

	BOOL IsEmpty() const; // Tests whether a vector contains no non-empty comps
	void Empty(); // Forces a vector to have 0 length (all empty comps)
	void Fill( LkCompT Comp ); // Fill all comps with one value
	LkCoord Replace( LkCompT OldComp, LkCompT NewComp ); // Change all comps with one value onto another
	LkCoord Count( LkCompT Comp ) const; // Count the number of comps with given value

	const LkVector<LkCompT>* GetData() const; // Get direct access to the components
	//LkCompT* GetBuffer() const; 
	//operator (LkCompT*)() const; // Directly accesses comps stored in a vector as an array

	LkError FreeExtra(); // Free extra memory allocated for a vector

	//
	// Component (assignment) operations
	//
	const LkVectorVector<LkCompT>& operator=( const LkVectorVector<LkCompT>& VVectorSrc ); 
	const LkVectorVector<LkCompT>& operator=( const LkCompT CompSrc ); // The same as Fill(), EmptyComp is not changed



};

#endif // !defined(__LkVectorVector_h__)
