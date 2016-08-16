#include "stdafx.h"
#include "Lk.h"

//
// Explicit istantiation of necessary classes
//
template class LkVector<LkCompByte>;
template class LkVector<LkCompWord>;
template class LkVector<LkCompLong>;
// template class LkVector<LkCompHuge>;
template class LkVector<LkCompFloat>;
template class LkVector<LkCompDouble>;

//
// Construction
//

template <class LkCompT>
LkVector<LkCompT>::LkVector()
{
	m_Data = NULL; // No components
	m_Size = 0;
	m_EmptyComp = 0;
	m_Length = 0;
	m_GrowBy = 1;
}

template <class LkCompT>
LkVector<LkCompT>::LkVector( LkCoord Size, LkCompT EmptyComp = 0, LkCoord GrowBy = 1 )
{
	if ( Size < LkCoord_Min || Size > LkCoord_Max )
		; // ERROR: Bad size

	m_Size = Size;

	if ( GrowBy < 1 || GrowBy > LkCoord_Max )
		; // ERROR: Bad increment value

	m_GrowBy = GrowBy;

	m_Data = (LkCompT*) GlobalAlloc( GPTR, (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkCompT) );
	if ( m_Data == NULL )
		; // ERROR: Memory

	m_EmptyComp = EmptyComp;
	m_Length = 0;
	// Initialize components with empty value
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		m_Data[comp] = m_EmptyComp;
}

//
// Destruction
//

template <class LkCompT>
LkVector<LkCompT>::~LkVector() 
{
	if ( m_Data != NULL )
		GlobalFree( m_Data );

	m_Data = NULL;
	m_Size = 0;
	m_EmptyComp = 0;
	m_Length = 0;
	m_GrowBy = 1;
}

//
// General functions
//

template <class LkCompT>
LkCoord LkVector<LkCompT>::GetSize() const
{
	return m_Size;
}

template <class LkCompT>
LkCoord LkVector<LkCompT>::SetSize( LkCoord Size, LkCoord GrowBy )
{
	if ( Size < LkCoord_Min || Size > LkCoord_Max )
		return LkCoord_Null; // ERROR: Bad size

	if ( GrowBy < 1 || GrowBy > LkCoord_Max )
		return LkCoord_Null; // ERROR: Bad increment value

	m_GrowBy = GrowBy; // Set new increment

	if ( Size == m_Size )
		return m_Size; // No change is necessary

	LkCoord OldSize = m_Size;
	LkCoord comp;
	LkCompT *Data;

	if ( Size < m_Size )
	{
		if ( Size == 0 )
		{
			m_Length = 0;
		}
		else
		{
			// Recalculate m_Length
			for ( comp = Size; comp < m_Size; comp++ )
				if ( m_Data[comp] != m_EmptyComp ) m_Length--;
		}
	}
	else // ! ( Size < m_Size )
	{
		if ( m_Data == NULL )
		{
			// Allocate new memory for comps
			Data = (LkCompT*) GlobalAlloc( GPTR, (Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkCompT) );
			if ( Data == NULL )
				return LkCoord_Null; // ERROR: Memory
		}
		else // ! ( m_Data == NULL )
		{
			// Check if new memory is necessary
			if ( Size*sizeof(LkCompT) > GlobalSize( m_Data ) )
			{
				// ReAllocate new memory 
				Data = (LkCompT*) GlobalReAlloc( (HGLOBAL) m_Data, (Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkCompT), GMEM_MOVEABLE );
				if ( Data == NULL )
					return LkCoord_Null; // ERROR: Memory
			}
		}

		m_Data = Data;

		// Initialize additional components with empty value
		for ( LkCoord comp = m_Size; comp < Size; comp++ )
			m_Data[comp] = m_EmptyComp;

	}

	m_Size = Size;
	return OldSize;
}

template <class LkCompT>
LkCompT LkVector<LkCompT>::GetEmptyComp() const
{
	return m_EmptyComp;
}

template <class LkCompT>
LkCompT LkVector<LkCompT>::SetEmptyComp( LkCompT EmptyComp )
// Previous empty comps take new value (stay empty)
// Alternative: previous empty comps save old value (bacome non-empty)
{
	LkCompT OldEmptyComp = m_EmptyComp;

	// Initialize old empty comps with new empty value and count new empty comps
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		if ( m_Data[comp] == m_EmptyComp ) 
			m_Data[comp] = EmptyComp;
		else if ( m_Data[comp] == EmptyComp ) 
			m_Length++;

	m_EmptyComp = EmptyComp;
	return OldEmptyComp;
}

template <class LkCompT>
LkCoord LkVector<LkCompT>::GetLength() const
{
	return m_Length;
}

template <class LkCompT>
void LkVector<LkCompT>::SetLength( LkCoord Length )
{
	m_Length = Length;
}

template <class LkCompT>
void LkVector<LkCompT>::IncrLength()
{
	m_Length++;
}

template <class LkCompT>
void LkVector<LkCompT>::DecrLength()
{
	m_Length--;
}

template <class LkCompT>
LkCoord LkVector<LkCompT>::UpdateLength()
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		if ( m_Data[comp] != m_EmptyComp )
			m_Length++;  // Count all non-empty comps

	return m_Length;
}

template <class LkCompT>
BOOL LkVector<LkCompT>::IsEmpty() const
{
	return (m_Length == 0);
}

template <class LkCompT>
void LkVector<LkCompT>::Empty()
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		m_Data[comp] = m_EmptyComp;

	m_Length = 0;
}

template <class LkCompT>
void LkVector<LkCompT>::Fill( LkCompT Comp )
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		m_Data[comp] = Comp;

	if ( Comp == m_EmptyComp )
		m_Length = 0;
	else
		m_Length = m_Size;
}

template <class LkCompT>
LkCoord LkVector<LkCompT>::Replace( LkCompT OldComp, LkCompT NewComp )
{
	LkCoord New = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		if ( m_Data[comp] == OldComp )
		{
			m_Data[comp] = NewComp;
			New++;
		}

	if ( OldComp == NewComp )
		;
	else if ( OldComp == m_EmptyComp )
		m_Length -= New;
	else if ( NewComp == m_EmptyComp )
		m_Length += New;

	return New;
}

template <class LkCompT>
LkCoord LkVector<LkCompT>::Count( LkCompT Comp ) const
{
	LkCoord Num = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
		if ( m_Data[comp] == Comp )
			Num++;

	return Num;
}

template <class LkCompT>
const LkCompT* LkVector<LkCompT>::GetData() const
{
	return m_Data;
}

template <class LkCompT>
LkError LkVector<LkCompT>::FreeExtra()
{
	if ( (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkCompT) >= GlobalSize( m_Data ) )
		return LkError_Success; // No extra (unused) memory

	// ReAllocate new memory 
	LkCompT *Data = (LkCompT*) GlobalReAlloc( (HGLOBAL) m_Data, (m_Size/m_GrowBy + 1)*m_GrowBy*sizeof(LkCompT), GMEM_MOVEABLE );
	if ( Data == NULL )
		return LkError_Memory; // ERROR: Memory

	m_Data = Data;

	return LkError_Success;
}

//
// Component (assignment) operations
//

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator=( const LkVector<LkCompT>& VectorSrc )
{
	LkCompT *Data;

	if ( m_Data == NULL )
	{
		// Allocate new memory 
		Data = (LkCompT*) GlobalAlloc( GPTR, (VectorSrc.m_Size/VectorSrc.m_GrowBy + 1)*VectorSrc.m_GrowBy*sizeof(LkCompT) );
		if ( Data == NULL )
			; // return NULL; // ERROR: Memory
	}
	else // Data != NULL
	{
		// Check if new memory is necessary
		if ( VectorSrc.m_Size*sizeof(LkCompT) > GlobalSize( m_Data ) )
		{
			// ReAllocate new memory 
			Data = (LkCompT*) GlobalReAlloc( (HGLOBAL) m_Data, (VectorSrc.m_Size/VectorSrc.m_GrowBy + 1)*VectorSrc.m_GrowBy*sizeof(LkCompT), GMEM_MOVEABLE );
			if ( Data == NULL )
				; // return NULL; // ERROR: Memory
		}
	}

	m_Data = Data;

	// Copy the data (components) themselves
	CopyMemory( m_Data, VectorSrc.m_Data, VectorSrc.m_Size*sizeof(LkCompT) );

	// Copy the object itself
	m_Size = VectorSrc.m_Size;

	m_EmptyComp = VectorSrc.m_EmptyComp;
	m_Length = VectorSrc.m_Length;

	m_GrowBy = VectorSrc.m_GrowBy;

	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator=( LkCompT CompSrc )
{
	Fill( CompSrc );
	return *this;
}

template <class LkCompT>
LkCompT LkVector<LkCompT>::GetAt( LkCoord Coord ) const
{
	return m_Data[Coord];
}

template <class LkCompT>
LkCompT LkVector<LkCompT>::operator[]( LkCoord Coord ) const
{
	return m_Data[Coord];
}

template <class LkCompT>
void LkVector<LkCompT>::SetAt( LkCoord Coord, LkCompT Comp )
{
	m_Data[Coord] = Comp;
}

template <class LkCompT>
void LkVector<LkCompT>::SetAtEx( LkCoord Coord, LkCompT Comp )
{
	if ( m_Data[Coord] == m_EmptyComp )
		if ( (m_Data[Coord] = Comp) != m_EmptyComp )
			m_Length--;
	else
		if ( (m_Data[Coord] = Comp) == m_EmptyComp )
			m_Length++;
}

template <class LkCompT>
void LkVector<LkCompT>::Include( LkCoord Coord )
{
	m_Data[Coord] = 1;
}

template <class LkCompT>
void LkVector<LkCompT>::IncludeEx( LkCoord Coord )
{
	if ( m_Data[Coord] == m_EmptyComp )
		if ( (m_Data[Coord] = 1) != m_EmptyComp )
			m_Length--;
	else
		if ( (m_Data[Coord] = 1) == m_EmptyComp )
			m_Length++;
}

template <class LkCompT>
void LkVector<LkCompT>::Exclude( LkCoord Coord )
{
	m_Data[Coord] = 0;
}

template <class LkCompT>
void LkVector<LkCompT>::ExcludeEx( LkCoord Coord )
{
	if ( m_Data[Coord] == m_EmptyComp )
		if ( (m_Data[Coord] = 0) != m_EmptyComp )
			m_Length--;
	else
		if ( (m_Data[Coord] = 0) == m_EmptyComp )
			m_Length++;
}

//
// Pointwise operations
//

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator&=( const LkVector<LkCompT>& VectorSrc )
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( m_Data[comp] < VectorSrc.m_Data[comp] ) // MIN
			continue;
		if ( (m_Data[comp] = VectorSrc.m_Data[comp]) == m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator&=( const LkCompT CompSrc )
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( m_Data[comp] < CompSrc ) // MIN
			continue;
		if ( (m_Data[comp] = CompSrc) == m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator|=( const LkVector<LkCompT>& VectorSrc )
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( m_Data[comp] > VectorSrc.m_Data[comp] ) // MAX
			continue;
		if ( (m_Data[comp] = VectorSrc.m_Data[comp]) == m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator|=( const LkCompT CompSrc )
{
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( m_Data[comp] > CompSrc ) // MAX
			continue;
		if ( (m_Data[comp] = CompSrc) == m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator*=( const LkVector<LkCompT>& VectorSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] *= VectorSrc.m_Data[comp]) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator*=( const LkCompT CompSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] *= CompSrc) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator/=( const LkVector<LkCompT>& VectorSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] /= VectorSrc.m_Data[comp]) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator/=( const LkCompT CompSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] /= CompSrc) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator+=( const LkVector<LkCompT>& VectorSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] += VectorSrc.m_Data[comp]) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator+=( const LkCompT CompSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] += CompSrc) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator-=( const LkVector<LkCompT>& VectorSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] -= VectorSrc.m_Data[comp]) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

template <class LkCompT>
const LkVector<LkCompT>& LkVector<LkCompT>::operator-=( const LkCompT CompSrc )
{
	m_Length = 0;
	for ( LkCoord comp = 0; comp < m_Size; comp++ )
	{
		if ( (m_Data[comp] -= CompSrc) != m_EmptyComp )
			m_Length++;
	}
	return *this;
}

//
// Pointwise comparison
//

template <class LkCompT>
BOOL operator==( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] != v1.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator==( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 != v2.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator==( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] != c2 )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator!=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] != v1.m_Data[comp] )
			return TRUE;
	}
	return FALSE;
}

template <class LkCompT>
BOOL operator!=( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 != v2.m_Data[comp] )
			return TRUE;
	}
	return FALSE;
}

template <class LkCompT>
BOOL operator!=( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] != c2 )
			return TRUE;
	}
	return FALSE;
}

template <class LkCompT>
BOOL operator<( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] >= v1.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator<( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 >= v2.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator<( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] >= c2 )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator<=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] > v1.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator<=( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 > v2.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator<=( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] > c2 )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] <= v1.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 <= v2.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] <= c2 )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>=( const LkVector<LkCompT>& v1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] < v1.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>=( const LkCompT c1, const LkVector<LkCompT>& v2 )
{
	for ( LkCoord comp = 0; comp < v2.m_Size; comp++ )
	{
		if ( c1 < v2.m_Data[comp] )
			return FALSE;
	}
	return TRUE;
}

template <class LkCompT>
BOOL operator>=( const LkVector<LkCompT>& v1, const LkCompT c2 )
{
	for ( LkCoord comp = 0; comp < v1.m_Size; comp++ )
	{
		if ( v1.m_Data[comp] < c2 )
			return FALSE;
	}
	return TRUE;
}

