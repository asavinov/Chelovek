#include "stdafx.h"
#include "Lk.h"

//
// Construction
//

template <class LkCompT>
LkVectorVector<LkCompT>::LkVectorVector()
{
	m_Data = NULL; // No vectors 
	m_Size = 0;
	m_Length = 0;
	m_GrowBy = 1;
	m_TotLength = 0;
}

template <class LkCompT>
LkVectorVector<LkCompT>::LkVectorVector( LkCoord Size, LkCoord GrowBy )
{
	if ( Size < LkCoord_Min || Size > LkCoord_Max )
		; // ERROR: Bad size

	m_Size = Size;

	if ( GrowBy < 1 || GrowBy > LkCoord_Max )
		; // ERROR: Bad increment value

	m_GrowBy = GrowBy;

	m_Data = (LkVector<LkCompT>*) GlobalAlloc( GPTR, (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>) );
	if ( m_Data == NULL )
		; // ERROR: Memory

	m_Length = 0;

	// Initialize m_Size vectors (all non-existent, i.e., repeat default constructor for LkVector) 
	LkVector<LkCompT> *Data;
	LkCoord sect;
	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		Data->m_Data = NULL;
		Data->m_Size = 0;
		Data->m_EmptyComp = 0;
		Data->m_Length = 0;
		Data->m_GrowBy = 1;
	}

	m_TotLength = 0;

}

template <class LkCompT>
LkVectorVector<LkCompT>::LkVectorVector( LkVector<LkCompT> &Sizes, LkCompT EmptyComp, LkCoord GrowBy )
{

	if ( Sizes.GetSize() < LkCoord_Min || Sizes.GetSize() > LkCoord_Max )
		; // ERROR: Bad size (number of sections)

	m_Size = Sizes.GetSize();

	LkCoord sect;
	for ( sect = 0; sect < m_Size; sect++ )
		if ( Sizes.GetAt(sect) < LkCoord_Min || Sizes.GetAt(sect) > LkCoord_Max )
			; // ERROR: Bad section size

	if ( GrowBy < 1 || GrowBy > LkCoord_Max )
		; // ERROR: Bad increment value

	m_GrowBy = 1;

	m_Data = (LkVector<LkCompT>*) GlobalAlloc( GPTR, (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>) );
	if ( m_Data == NULL )
		; // ERROR: Memory

	m_Length = 0;

	// Initialize m_Size vectors (repeat a constructor for LkVector)
	// Alternative: use SetSize() for each section (more simple but less effective)
	LkVector<LkCompT> *Data;
	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		Data->m_Size = Sizes.GetAt(sect);
		Data->m_GrowBy = GrowBy; // The same for all sections -- should be changed explicitely if necessary

		Data->m_Data = (LkCompT*) GlobalAlloc( GPTR, (Data->m_Size/Data->m_GrowBy + 1)*Data->m_GrowBy*sizeof(LkCompT) );
		if ( Data->m_Data == NULL )
			; // ERROR: Memory

		Data->m_EmptyComp = EmptyComp;  // The same for all sections -- should be changed explicitely if necessary
		Data->m_Length = 0;
		// Initialize components with empty value
		for ( LkCoord comp = 0; comp < Data->m_Size; comp++ )
			Data->m_Data[comp] = m_EmptyComp;
	}

	m_TotLength = 0;
}

//
// Destruction
//

template <class LkCompT>
LkVectorVector<LkCompT>::~LkVectorVector() 
{
	// Delete m_Size vectors (repeat destructor for LkVector)
	// Alternative: use ~LkVector() for each section 
	for ( LkCoord sect = 0; sect < m_Size; sect++ )
	{
		if ( m_Data[sect].m_Data != NULL )
			GlobalFree( m_Data[sect].m_Data );
	}

	if ( m_Data != NULL )
		GlobalFree( m_Data );

	m_Data = NULL;
	m_Size = 0;
	m_Length = 0;
	m_GrowBy = 1;
	m_TotLength = 0;
}

//
// General functions
//

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::GetSize() const
{
	return m_Size;
}

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::SetSize( LkCoord Size, LkCoord GrowBy )
{
	if ( Size < LkCoord_Min || Size > LkCoord_Max )
		return LkCoord_Null; // ERROR: Bad size

	if ( GrowBy < 1 || GrowBy > LkCoord_Max )
		return LkCoord_Null; // ERROR: Bad increment value

	m_GrowBy = GrowBy; // Set new increment

	if ( Size == m_Size )
		return m_Size; // No change is necessary

	LkCoord OldSize = m_Size;
	LkCoord sect;
	LkVector<LkCompT> *Data;

	if ( Size < m_Size )
	{
		// Recalculate m_Length, m_TotLength and empty ending (tail) sections
		for ( sect = Size, Data = m_Data+sect; sect < m_Size; sect++, Data++ )
		{
			if ( ! Data->IsEmpty() ) m_Length--;
			m_TotLength -= Data->GetLength();
			Data->~LkVector();
		}
		if ( Size == 0 ) // Not necessary -- it is an additional gguarantee of correctness
			m_Length = 0;
	}
	else // Size >= m_Size )
	{
		if ( m_Data == NULL )
		{
			// Allocate new memory for sections 
			Data = (LkVector<LkCompT>*) GlobalAlloc( GPTR, (Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>) );
			if ( Data == NULL )
				return LkCoord_Null; // ERROR: Memory
		}
		else // m_Data != NULL
		{
			// Check if new memory is necessary
			if ( Size*sizeof(LkVector<LkCompT>) > GlobalSize( m_Data ) )
			{
				// ReAllocate new memory 
				Data = (LkVector<LkCompT>*) GlobalReAlloc( (HGLOBAL) m_Data, (Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>), GMEM_MOVEABLE );
				if ( Data == NULL )
					return LkCoord_Null; // ERROR: Memory
			}
		}

		m_Data = Data;

		// Initialize additional sections with empty values (no sections)
		for ( sect = m_Size, Data = m_Data+sect; sect < Size; sect++, Data++ )
		{
			Data->m_Data = NULL;
			Data->m_Size = 0;
			Data->m_EmptyComp = 0;
			Data->m_Length = 0;
			Data->m_GrowBy = 1;
		}

	}

	m_Size = Size;
	return OldSize;
}

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::GetLength() const
{
	return m_Length;
}

template <class LkCompT>
void LkVectorVector<LkCompT>::SetLength( LkCoord Length )
{
	m_Length = Length;
}

template <class LkCompT>
void LkVectorVector<LkCompT>::IncrLength()
{
	m_Length++;
}

template <class LkCompT>
void LkVectorVector<LkCompT>::DecrLength()
{
	m_Length--;
}

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::UpdateLength()
{
	LkCoord sect;
	LkVector<LkCompT> *Data;

	m_Length = 0;
	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
		if ( ! Data->IsEmpty() ) 
			m_Length++; // Count all non-empty sections

	return m_Length;
}

template <class LkCompT>
BOOL LkVectorVector<LkCompT>::IsEmpty() const
{
	return (m_Length == 0);
}

template <class LkCompT>
void LkVectorVector<LkCompT>::Empty()
{
	LkCoord sect, comp;
	LkVector<LkCompT> *Data;

	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		if ( Data->m_Data == NULL ) continue;
		for ( comp = 0; comp < Data->m_Size; comp++ )
			Data->m_Data[comp] = Data->m_EmptyComp;
		Data->m_Length = 0;
	}

	m_Length = 0;
	m_TotLength = 0;
}

template <class LkCompT>
void LkVectorVector<LkCompT>::Fill( LkCompT Comp )
{
	LkCoord sect, comp;
	LkVector<LkCompT> *Data;

	m_Length = 0;
	m_TotLength = 0;

	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		if ( Data->m_Data == NULL ) continue;
		for ( comp = 0; comp < Data->m_Size; comp++ )
			Data->m_Data[comp] = Comp;

		if ( Comp == Data->m_EmptyComp )
			Data->m_Length = 0;
		else
		{
			Data->m_Length = Data->m_Size;
			m_Length++;
			m_TotLength += Data->m_Size;
		}
	}

}

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::Replace( LkCompT OldComp, LkCompT NewComp )
{
	LkCoord sect;
	LkVector<LkCompT> *Data;

	m_Length = 0;
	m_TotLength = 0;

	LkCoord New = 0;

	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		if ( Data->m_Data == NULL ) continue;

		New += Data->Replace( OldComp, NewComp );

		if ( Data->m_Length != 0 ) m_Length++;
		m_TotLength += Data->m_Length;
	}

	return New;
}

template <class LkCompT>
LkCoord LkVectorVector<LkCompT>::Count( LkCompT Comp ) const
{
	LkCoord sect, comp;
	LkVector<LkCompT> *Data;

	LkCoord Num = 0;

	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		if ( Data->m_Data == NULL ) continue;
		for ( comp = 0; comp < Data->m_Size; comp++ )
			if ( Data->m_Data[comp] == Comp )
				Num++;
	}

	return Num;
}

template <class LkCompT>
const LkVector<LkCompT>* LkVectorVector<LkCompT>::GetData() const
{
	return m_Data;
}

template <class LkCompT>
LkError LkVectorVector<LkCompT>::FreeExtra()
{

	LkCoord sect;
	LkVector<LkCompT> *Data;
	LkError Error;

	for ( sect = 0, Data = m_Data; sect < m_Size; sect++, Data++ )
	{
		if ( Data->m_Data == NULL ) continue;
		if ( Data->FreeExtra() != LkError_Success )
			return LkError_Memory; // ERROR: Memory
	}

	// Free extra memory for sections 

	if ( (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>) >= GlobalSize( m_Data ) )
		return LkError_Success; // No extra (unused) memory

	// ReAllocate new memory 
	Data = (LkVector<LkCompT>*) GlobalReAlloc( (HGLOBAL) m_Data, (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkVector<LkCompT>), GMEM_MOVEABLE );
	if ( Data == NULL )
		return LkError_Memory; // ERROR: Memory

	m_Data = Data;

	return LkError_Success;
}

//
// Component (assignment) operations
//

template <class LkCompT>
const LkVectorVector<LkCompT>& LkVectorVector<LkCompT>::operator=( const LkVectorVector<LkCompT>& VVectorSrc )
{
	LkVector<LkCompT> *Data;

	if ( m_Data == NULL )
	{
		// Allocate new memory 
		Data = (LkVector<LkCompT>*) GlobalAlloc( GPTR, (VVectorSrc.m_Size/VVectorSrc.m_GrowBy + 1)*VVectorSrc.m_GrowBy*sizeof(LkVector<LkCompT>) );
		if ( Data == NULL )
			return NULL; // ERROR: Memory
	}
	else // m_Data != NULL
	{
		// Check if new memory is necessary
		if ( VVectorSrc.m_Size*sizeof(LkVector<LkCompT>) > GlobalSize( m_Data ) )
		{
			// ReAllocate new memory 
			Data = (LkVector<LkCompT>*) GlobalReAlloc( (HGLOBAL) m_Data, (VVectorSrc.m_Size/VVectorSrc.m_GrowBy + 1)*VVectorSrc.m_GrowBy*sizeof(LkVector<LkCompT>), GMEM_MOVEABLE );
			if ( Data == NULL )
				return NULL; // ERROR: Memory
		}
	}

	m_Data = Data;

	LkCoord sect;

	if ( m_Size < VVectorSrc.m_Size )
		// Zero all tail (new allocated) sections (in fact, it's enough m_Data = 0)
		for ( sect = m_Size, Data = m_Data+sect; sect < VVectorSrc.m_Size; sect++, Data++ )
			Data->m_Data = Data->m_Size = Data->m_EmptyComp = Data->m_Length = Data->m_GrowBy = 0;
	else // m_Size >= VVectorSrc.m_Size
		// Delete all already unnecessary sections 
		for ( sect = VVectorSrc.m_Size, Data = m_Data+sect; sect < m_Size; sect++, Data++ )
			Data->~LkVector();

	// For each section call assignment operator (copy each section)
	for ( sect = 0, Data = m_Data; sect < VVectorSrc.m_Size; sect++, Data++ )
		m_Data[sect] = VVectorSrc.m_Data[sect];

	// Copy the object itself
	m_Size = VVectorSrc.m_Size;
	m_Length = VVectorSrc.m_Length;
	m_GrowBy = VVectorSrc.m_GrowBy;
	m_TotLength = VVectorSrc.m_TotLength;

	return *this;
}

template <class LkCompT>
const LkVectorVector<LkCompT>& LkVectorVector<LkCompT>::operator=( const LkCompT CompSrc )
{
	Fill( CompSrc );
	return *this;
}

