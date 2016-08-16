#if !defined(__LkVector_h__)
#define __LkVector_h__

/*
Vector is mainly intended for manipulating distribution values 
since the operation of changing the size is not very effective.

Table is especially effective when used as a set, i.e., 
two-valued distribution with fixed (maximum) size. When including 
elements we have also to indicate its degree of membership 
(distribution value). In the case of bit table it is enough 
only to include/exclude.

Use vector instead of table when size is fixed or changed rarely.
When the distribution size is changed frequently use table.

There is a problem consisting in that sometimes after 
changing the vector it is nor required to update its length 
because it is not interesting at all (e.g., for temporary 
vectors or when this field is not used) or when it is faster 
to update this field after all modifications has been made 
(e.g., after intensive and complex modifications or when we 
are able to use significantly our own information about the 
vector state and the operation to be applied to optimize the 
updating the field). Therefore all intensive vector operations 
do not update the length. If it is necessary to update the 
length there should be used functions edning with Ex.
Thus usual functions do not update this field at all and it is 
up to the user to update it when necessary and if it is 
required (e.g., after the object has been changed). In 
addition, updating is required after direct access (with 
changes) to the vector components. Updating is fulfilled 
through the UpdateLength(), SetLength(), IncrLength() and 
DecrLength() functions.

Since the use of the vector class is limited by low level 
programming (at higher levels more complex objects are 
used which wrap the vector as a subobject) most functins 
are inline.
*/

template <class LkCompT> 
class LkVector
{
private:

	LkCompT *m_Data; // Pointer to the array of vector components
	LkCoord m_Size; // Power of the space (domain of definition)

	LkCompT m_EmptyComp; // Explicit value of empty component
	LkCoord m_Length; // Number of non-empty comps 

	LkCoord m_GrowBy; // Number of comps added when expanding the vector

public:

	//
	// Construction
	//
	LkVector(); // Default constructor
	LkVector( LkCoord Size, LkCompT EmptyComp, LkCoord GrowBy );

	//
	// Destruction
	//
	~LkVector();

	//
	// General functions
	//

	LkCoord GetSize() const; // N: The power of the space (domain of definition)
	LkCoord SetSize( LkCoord Size, LkCoord GrowBy = 1 ); // Set new size (reallocation only when necessary)

	LkCompT GetEmptyComp() const;
	LkCompT SetEmptyComp( LkCompT EmptyComp );

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

	const LkCompT* GetData() const; // Get direct access to the components
	//LkCompT* GetBuffer() const; 
	//operator (LkCompT*)() const; // Directly accesses comps stored in a vector as an array

	LkError FreeExtra(); // Free extra memory allocated for a vector

	//
	// Component (assignment) operations
	//

	const LkVector<LkCompT>& operator=( const LkVector<LkCompT>& VectorSrc ); 
	const LkVector<LkCompT>& operator=( LkCompT CompSrc ); // The same as Fill(), EmptyComp is not changed

	LkCompT GetAt( LkCoord Coord ) const; // Returns the comp at a given coordinate
	LkCompT operator []( LkCoord Coord ) const; // Returns the comp at a given coordinate — operator substitution for GetAt

	void SetAt( LkCoord Coord, LkCompT Comp ); // Sets a comp at a given position
	void SetAtEx( LkCoord Coord, LkCompT Comp ); // SetAt version with length update

	void Include( LkCoord Coord ); // SET substitution for SetAt (set to 1)
	void IncludeEx( LkCoord Coord ); // Include version with length update

	void Exclude( LkCoord Coord ); // SET substitution for SetAt (set to 0)
	void ExcludeEx( LkCoord Coord ); // Exclude version with length update

	//
	// Pointwise operations
	//
	// size must be equal

  // pointwise MIN
	const LkVector<LkCompT>& operator&=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator&=( const LkCompT CompSrc );

  // pointwise MAX
	const LkVector<LkCompT>& operator|=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator|=( const LkCompT CompSrc );

  // pointwise product
	const LkVector<LkCompT>& operator*=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator*=( const LkCompT CompSrc );
  
  // pointwise devision
	const LkVector<LkCompT>& operator/=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator/=( const LkCompT CompSrc );
  
	// pointwise sum
	const LkVector<LkCompT>& operator+=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator+=( const LkCompT CompSrc );

  // pointwise subtraction
	const LkVector<LkCompT>& operator-=( const LkVector<LkCompT>& VectorSrc );
	const LkVector<LkCompT>& operator-=( const LkCompT CompSrc );

	//
	// Pointwise comparison
	//
	// size must be equal
	// These make componentwise comparison only and do not compare lengths
	// so it makes sense to compare lengths before calling them

	friend BOOL operator==( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator==( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator==( const LkVector<LkCompT>& v1, const LkCompT c2 );

	friend BOOL operator!=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator!=( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator!=( const LkVector<LkCompT>& v1, const LkCompT c2 );

	friend BOOL operator<( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator<( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator<( const LkVector<LkCompT>& v1, const LkCompT c2 );

	friend BOOL operator<=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator<=( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator<=( const LkVector<LkCompT>& v1, const LkCompT c2 );

	friend BOOL operator>( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator>( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator>( const LkVector<LkCompT>& v1, const LkCompT c2 );

	friend BOOL operator>=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 );
	friend BOOL operator>=( const LkCompT c1, const LkVector<LkCompT>& v2 );
	friend BOOL operator>=( const LkVector<LkCompT>& v1, const LkCompT c2 );

	//
	// Aggregation (projection, consolidation) functions
	//
/*
	Min();
	Max();
	Inf();
	Mean();
*/
	//
	// Extraction, insertion and concationation (with ranges)
	//

	// Add, Append, Delete
  // InsertAt (one vector or comp into another at any coord)
  // RemoveAt (range or comp from vector from within the vector)
	// Mid, Left, Right, TrimLeft, TrimRight

	//
	// Finding backward vector where the domain of definition and distriubtion are changed
	// (cross-tab vector?)
	//

	//
	// Archive/Dump
	//

};

#endif // !defined(__LkVector_h__)

// Bit -- vector of any number of bits
// #include "LkVectorBit.h"

// BitByte -- max 8 bits
// BitWord -- max 16 bits 
// BitLong -- max 32 bits 
// BitHuge -- max 64 bits 
// #include "LkVectorBitType.h"

/*
// Flags

LkTable_Squeeze // Squeeze empty space (empty comps) when necessary
LkTable_Order // Order comps when necessary

UpdateMin()
UpdateMax()
UpdateMean()
UpdateInf()
UpdateConst+()
UpdateConst*()

// Keeping parameters in actual state
LkDistr_Min
LkDistr_Max
LkDistr_Inf
Distr_Mid

*/

// Aggreagtion (projection) operations may have range of coordinates (or the whole distr if not indicated)
