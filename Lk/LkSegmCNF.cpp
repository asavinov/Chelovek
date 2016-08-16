#include "stdafx.h"
#include "Lk.h"

//
// Construction
//

LkSegmCNF::LkSegmCNF( LkVector<LkCoord> &Sizes, LkCoord MaxLines, LkCoord GoalVar, LkCoord MaxRank, LkCoord MaxTotRank )
{
	LkCoord var, segm, sect, line; 

	//
	// Check the validity of parameters 
	//

	if ( Sizes.GetSize() < 1 || Sizes.GetSize() > LkCoord_Max )
		; // ERROR: Bad number of variables 

	for ( var = 0; var < Sizes.GetSize(); var++ )
		if ( Sizes.GetAt(var) < 2 || Sizes.GetAt(var) > LkCoord_Max )
		; // ERROR: Bad number of values 

	if ( MaxLines < Sizes.GetSize() || MaxLines > LkCoord_Max )
		; // ERROR: Bad max number of lines 

	if ( (GoalVar < 0 || GoalVar >= Sizes.GetSize()) && GoalVar != LkCoord_Null )
		; // ERROR: Bad goal variable 

	if ( MaxRank < 1 )
		; // ERROR: Bad MaxRank

	if ( MaxTotRank < 1 )
		; // ERROR: Bad TotMaxRank

	//
	// First initialize syntax part 
	//

	m_Width = Sizes.GetSize(); // Number of variables (width of the matrix)

	m_Syn = (LkSegmSyn*) GlobalAlloc( GPTR, m_Width*sizeof(LkSegmSyn) );
	if ( m_Syn == NULL ) 
		; // ERROR: Memory

	// Initialize each element of the new syntax array 
	for ( var = 0; var < m_Width; var++ )
	{
		m_Syn[var].m_Size = Sizes.GetAt(var);

		if ( var == 0 ) m_Syn[var].m_AccuSum = 0;
		else m_Syn[var].m_AccuSum = m_Syn[var-1].m_AccuSum + m_Syn[var-1].m_Size;

		if ( var == 0 ) m_Syn[m_Width-1].m_AccuProd = 1;
		else m_Syn[m_Width-1-var].m_AccuProd = m_Syn[m_Width-var].m_AccuProd * m_Syn[var-1].m_Size;

		m_Syn[var].m_PointCoord = LkCoord_Null; // Not used now (only during calculations)
		m_Syn[var].m_PointOffset = LkCoord_Null; // Not used now (only during calculations)

		// The same in segments (offsets or lengths)
		m_Syn[var].m_SizeB = (m_Syn[var].m_Size - 1) / (sizeof(LkSegm)*8) + 1;
		if ( var == 0 ) m_Syn[var].m_AccuSumB = 0;
		else m_Syn[var].m_AccuSumB = m_Syn[var-1].m_AccuSumB + m_Syn[var-1].m_SizeB;
		if ( var == 0 ) m_Syn[m_Width-1].m_AccuProdB = 1;
		else m_Syn[m_Width-1-var].m_AccuProdB = m_Syn[m_Width-var].m_AccuProdB * m_Syn[var-1].m_SizeB;

		m_Syn[var].m_PointCoordB = LkCoord_Null;
		m_Syn[var].m_PointOffsetB = LkCoord_Null;

		m_Syn[var].m_PointSegm = 0;
	}

	m_LineWidth = m_Syn[m_Width-1].m_AccuSum + m_Syn[m_Width-1].m_Size;
	m_LineWidthB = m_Syn[m_Width-1].m_AccuSumB + m_Syn[m_Width-1].m_SizeB;

	m_Min = 0;
	m_Max = 0;

	m_NumLines = 1; // one empty line 

	m_MaxLines = MaxLines;
	m_GoalVar = GoalVar;
	m_MaxRank = MaxRank;
	m_MaxTotRank = MaxTotRank;

	//
	// Allocate memory
	//

	m_Comps = (LkSegm*) GlobalAlloc( GPTR, m_LineWidthB*(m_MaxLines+1)*sizeof(LkSegm) );
	if ( m_Comps == NULL ) 
		; // ERROR: Memory
	
	m_Sects = (LkSegmSect*) GlobalAlloc( GPTR, m_Width*(m_MaxLines+1)*sizeof(LkSegmSect) );
	if ( m_Sects == NULL ) 
		; // ERROR: Memory

	m_Lines = (LkSegmLine*) GlobalAlloc( GPTR, (m_MaxLines+1)*sizeof(LkSegmLine) );
	if ( m_Lines == NULL ) 
		; // ERROR: Memory

	//
	// Initialize line headers
	//

	for ( line = 0; line < m_MaxLines+1; line++ )
	{
		m_Lines[line].m_Comps = m_Comps + m_LineWidthB*line;

		m_Lines[line].m_Sects = m_Sects + m_Width*line;

		m_Lines[line].m_Length = 0;
		m_Lines[line].m_TotLength = 0;

		m_Lines[line].m_Const = 0;
		m_Lines[line].m_CompSum = 0;
		m_Lines[line].m_Info = 0;

		unsigned char m_Flags = 0;

		// Parameters of generation
		m_Lines[line].m_OldVar = LkCoord_Null;
		m_Lines[line].m_OldComp = 0;
		m_Lines[line].m_OldCompOffs = LkCoord_Null;

		// Main lists
		m_Lines[line].m_InfoNext = & m_Lines[line+1];
		m_Lines[line].m_InfoPrev = & m_Lines[line-1];

		m_Lines[line].m_CompSumNext = & m_Lines[line+1];
		m_Lines[line].m_CompSumPrev = & m_Lines[line-1];

		// Sublists (absent)
		m_Lines[line].m_SubNext = & m_Lines[line+1];
		m_Lines[line].m_SubPrev = & m_Lines[line-1];

		m_Lines[line].m_SubListType = LkSegmLine_FreeList;

		//
		// Initialize section headers
		//
		for ( sect = 0; sect < m_Width; sect++ )
		{
			m_Lines[line].m_Sects[sect].m_Length = 0;
			m_Lines[line].m_Sects[sect].m_CompSum = 0;
			m_Lines[line].m_Sects[sect].m_Comps = m_Lines[line].m_Comps + m_Syn[sect].m_AccuSumB;

			//
			// Initialize semantic part (segments)
			//
			for ( segm = 0; segm < m_Syn[sect].m_SizeB; segm++ )
				m_Lines[line].m_Sects[sect].m_Comps[segm] = ~(( (LkSegm)1 << (m_Syn[var].m_Size - segm*sizeof(LkSegm)*8) ) - 1);

		}
	}

	// Close the main lists from both sides
	m_Lines[0].m_InfoPrev = NULL;
	m_Lines[0].m_InfoNext = NULL;

	m_Lines[0].m_CompSumPrev = NULL;
	m_Lines[0].m_CompSumNext = NULL;

	m_Lines[1].m_SubPrev = NULL;
	m_Lines[m_MaxLines].m_SubNext = NULL;

	// 
	// Initialize ordered lists 
	//
	m_InfoListFirst = & m_Lines[0]; 
	m_InfoListLast = & m_Lines[0];

	m_CompSumListFirst = & m_Lines[0];
	m_CompSumListLast = & m_Lines[0];

	m_Lines[0].m_SubListType = LkSegmLine_NoList;

	m_StabListFirst = NULL;
	m_StabListLast = NULL;

	m_OldListFirst = NULL;
	m_OldListLast = NULL;

	m_NewListFirst = NULL;
	m_NewListLast = NULL;

	m_FreeListFirst = & m_Lines[1];;
	m_FreeListLast = & m_Lines[m_MaxLines];

	// Diagnostics/testing
#ifdef _DIAG
	m_DiagAbsorbByStab = 0;
	m_DiagAbsorbByStab_Const = 0;
	m_DiagAbsorbByStab_Critical = 0;
	m_DiagAbsorbByStab_Sects = 0;
	m_DiagAbsorbByStab_Comps = 0;

	m_DiagAbsorbByNew = 0;
#endif // _DIAG
}

//
// Destruction
//

LkSegmCNF::~LkSegmCNF()
{
	if ( m_Syn != NULL ) 
		GlobalFree( m_Syn );

	if ( m_Comps != NULL ) 
		GlobalFree( m_Comps );

	if ( m_Lines != NULL ) 
		GlobalFree( m_Lines );

	if ( m_Sects != NULL ) 
		GlobalFree( m_Sects );
}

//
// Induction functions 
//

LkError LkSegmCNF::ProcessPoint( LkVector<LkCoord> &Point )
{

	LkSegmLine *Line1;
	LkSegmLine *Line2;
	LkSegmLine *BufLine;

	LkSegmSect *Sect1;

	LkCoord line, var;

	//
	// Prepare necessary structures
	//
	for ( var = 0; var < m_Width; var++ )
	{
		m_Syn[var].m_PointCoord = Point.GetAt( var );
		m_Syn[var].m_PointOffset = m_Syn[var].m_AccuSum + m_Syn[var].m_PointCoord;

		m_Syn[var].m_PointCoordB = m_Syn[var].m_PointCoord / (sizeof(LkSegm)*8);
		m_Syn[var].m_PointOffsetB = m_Syn[var].m_AccuSumB + m_Syn[var].m_PointCoordB;

		m_Syn[var].m_PointSegm = (LkSegm)1 << (m_Syn[var].m_PointCoord % (sizeof(LkSegm)*8));
	}

	//
	// Initialize sublists, i.e., divide all lines into three sets (with one empty)
	//

	m_StabListFirst = m_StabListLast = m_OldListFirst = m_OldListLast = m_NewListFirst = m_NewListLast = NULL; // Empty all sublists 

	for ( line = 0, Line1 = m_CompSumListFirst; line < m_NumLines; line++, Line1 = Line1->m_CompSumNext )
	{
		// Determine if this line absorbs (covers) the point 
		for ( var = 0; var < m_Width; var++ )
			if ( (Line1->m_Comps[ m_Syn[var].m_PointOffsetB ] & m_Syn[var].m_PointSegm) != 0 )
				break;

		if ( var < m_Width ) 
			StabListAppend( Line1 );
		else 
			OldListAppend( Line1 );
	}

	// ==========
	// Primary Loop !!! Pass through all lines in old sublist
	// ==========
	for ( Line1 = m_OldListFirst; Line1 != NULL; )
	{
    // ----------
    // Secondary loop !!! Pass through all sections of the current line 
    // ----------
		for ( var = 0, Sect1 = Line1->m_Sects; var < m_Width; var++, Sect1++ )
		{
			BufLine = m_FreeListFirst; // Buffer line

			// Generate new line by modifying the current one 
			Sect1->m_Comps[ m_Syn[var].m_PointCoordB ] |= m_Syn[var].m_PointSegm;

			//
			// Check if the constant has changed
			//
			if ( Sect1->m_Length == m_Syn[var].m_Size - 1 )
			{
				// Restore (unmodify) the line
				Sect1->m_Comps[ m_Syn[var].m_PointCoordB ] &= ~m_Syn[var].m_PointSegm;
				continue;
			}
			else // Constant has not changed
			{
				// NewInformation: Line1->m_Info + (LkCompFloat) 1 / (LkCompFloat) m_Syn[var].m_Size

				// Check if the new line is out of Info list 
				if ( InfoListIsOut( Line1->m_Info + (LkCompFloat) 1 / (LkCompFloat) m_Syn[var].m_Size, Line1->m_CompSum + 1 ) )
					{
						// Restore (unmodify) the line
						Sect1->m_Comps[ m_Syn[var].m_PointCoordB ] &= ~m_Syn[var].m_PointSegm;
						continue;
					}

				// Copy line into a buffer
				CopyLine( Line1, BufLine );

				// and recalculate some parameters
				if ( BufLine->m_Sects[var].m_Length++ == 0 )
					BufLine->m_Length++;
				BufLine->m_TotLength++;

				BufLine->m_CompSum += 1;
				BufLine->m_Info += (LkCompFloat) 1 / (LkCompFloat) m_Syn[var].m_Size;

				BufLine->m_Sects[var].m_CompSum += 1;

				//
				// Set the line parameters of generation used to optimize absorption
				//
				BufLine->m_OldVar = var;
				BufLine->m_OldComp = 0;
				BufLine->m_OldCompOffs = m_Syn[var].m_PointOffsetB;
			}

			// Restore (unmodify) the source line
			Sect1->m_Comps[ m_Syn[var].m_PointCoordB ] &= ~m_Syn[var].m_PointSegm;

			//
			// Check if new line satisfies restricting parameters
			//
			if ( BufLine->m_Length == m_MaxRank && m_GoalVar != LkCoord_Null )
				if ( BufLine->m_Sects[m_GoalVar].m_Length == 0 )
				{
					continue;
				}

			if ( BufLine->m_Length > m_MaxRank )
			{
				continue;
			}

			if ( BufLine->m_TotLength > m_MaxTotRank )
			{
				continue;
			}

			//
			// Check if the New line is absorbed by some Stable 
			//
			if ( IsAbsorbedByStab( BufLine ) ) 
			{
				continue;
			}

			//
			// Insert the buffer line into appropriate location in the lists 
			//

			// Delete from Free list
			FreeListDelete( BufLine );

			// Find location and insert into Info list
			Line2 = InfoListFind( BufLine, Line1 );
			if ( Line2 != NULL )
				InfoListInsert( BufLine, Line2 );
			else
				InfoListAppend( BufLine );

			// Find location and insert into CompSum list
			Line2 = CompSumListFind( BufLine, Line1 );
			if ( Line2 != NULL )
				CompSumListInsert( BufLine, Line2 );
			else
				CompSumListAppend( BufLine );

			// Find location and insert into New list
			Line2 = NewListFind( BufLine );
			if ( Line2 != NULL )
				NewListInsert( BufLine, Line2 );
			else
				NewListAppend( BufLine );

			// If necessary free one line
			if ( m_NumLines < m_MaxLines )
				m_NumLines++;
			else
			{
				Line2 = m_InfoListLast;

				InfoListDelete( Line2 );
				CompSumListDelete( Line2 );
				SubListDelete( Line2 );

				FreeListAppend( Line2 );
			}


		} // ---------- End of secondary loop

		//
		// Move the source unstable line to the end of list (after the buffer)
		//

		Line2 = Line1;
		Line1 = Line1->m_SubNext;

		InfoListDelete( Line2 );
		CompSumListDelete( Line2 );
		OldListDelete( Line2 );

		FreeListAppend( Line2 );

		m_NumLines--;

	} // ========== End of primary loop

	return LkError_Success;
}

void LkSegmCNF::InfoListAppend( LkSegmLine *Line )
{
	if ( m_InfoListFirst == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_InfoListFirst = m_InfoListLast = Line;
		Line->m_InfoNext = Line->m_InfoPrev = NULL;
	}
	else
	{
		Line->m_InfoPrev = m_InfoListLast;
		Line->m_InfoNext = NULL; // This is now the last line in the sublist

		m_InfoListLast->m_InfoNext = Line;
		m_InfoListLast = Line;
	}
}

void LkSegmCNF::CompSumListAppend( LkSegmLine *Line )
{
	if ( m_CompSumListFirst == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_CompSumListFirst = m_CompSumListLast = Line;
		Line->m_CompSumNext = Line->m_CompSumPrev = NULL;
	}
	else
	{
		Line->m_CompSumPrev = m_CompSumListLast;
		Line->m_CompSumNext = NULL; // This is now the last line in the sublist

		m_CompSumListLast->m_CompSumNext = Line;
		m_CompSumListLast = Line;
	}
}

void LkSegmCNF::StabListAppend( LkSegmLine *Line )
{
	if ( m_StabListFirst == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_StabListFirst = m_StabListLast = Line;
		Line->m_SubNext = Line->m_SubPrev = NULL;
	}
	else
	{
		Line->m_SubPrev = m_StabListLast;
		Line->m_SubNext = NULL; // This is now the last line in the sublist

		m_StabListLast->m_SubNext = Line;
		m_StabListLast = Line;
	}
	Line->m_SubListType = LkSegmLine_StabList;
}

void LkSegmCNF::OldListAppend( LkSegmLine *Line )
{
	if ( m_OldListFirst == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_OldListFirst = m_OldListLast = Line;
		Line->m_SubNext = Line->m_SubPrev = NULL;
	}
	else
	{
		Line->m_SubPrev = m_OldListLast;
		Line->m_SubNext = NULL; // This is now the last line in the sublist

		m_OldListLast->m_SubNext = Line;
		m_OldListLast = Line;
	}
	Line->m_SubListType = LkSegmLine_OldList;
}

void LkSegmCNF::NewListAppend( LkSegmLine *Line )
{
	if ( m_NewListFirst == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_NewListFirst = m_NewListLast = Line;
		Line->m_SubNext = Line->m_SubPrev = NULL;
	}
	else
	{
		Line->m_SubPrev = m_NewListLast;
		Line->m_SubNext = NULL; // This is now the last line in the sublist

		m_NewListLast->m_SubNext = Line;
		m_NewListLast = Line;
	}
	Line->m_SubListType = LkSegmLine_NewList;
}

void LkSegmCNF::FreeListAppend( LkSegmLine *Line )
{
	if ( m_FreeListLast == NULL ) // or Last (they are NULL together) the sublist is empty
	{
		m_FreeListFirst = m_FreeListLast = Line;
		Line->m_SubNext = Line->m_SubPrev = NULL;
	}
	else
	{
		Line->m_SubPrev = m_FreeListLast;
		Line->m_SubNext = NULL; // This is now the last line in the sublist

		m_FreeListLast->m_SubNext = Line;
		m_FreeListLast = Line;
	}
	Line->m_SubListType = LkSegmLine_FreeList;
}

void LkSegmCNF::InfoListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_InfoNext != NULL )
		Line->m_InfoNext->m_InfoPrev = Line->m_InfoPrev;
	else
		m_InfoListLast = Line->m_InfoPrev;

	if ( Line->m_InfoPrev != NULL )
		Line->m_InfoPrev->m_InfoNext = Line->m_InfoNext;
	else
		m_InfoListFirst = Line->m_InfoNext;
}

void LkSegmCNF::CompSumListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_CompSumNext != NULL )
		Line->m_CompSumNext->m_CompSumPrev = Line->m_CompSumPrev;
	else
		m_CompSumListLast = Line->m_CompSumPrev;

	if ( Line->m_CompSumPrev != NULL )
		Line->m_CompSumPrev->m_CompSumNext = Line->m_CompSumNext;
	else
		m_CompSumListFirst = Line->m_CompSumNext;
}

void LkSegmCNF::StabListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_SubNext != NULL )
		Line->m_SubNext->m_SubPrev = Line->m_SubPrev;
	else
		m_StabListLast = Line->m_SubPrev;

	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line->m_SubNext;
	else
		m_StabListFirst = Line->m_SubNext;

	Line->m_SubListType = LkSegmLine_NoList;
}

void LkSegmCNF::OldListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_SubNext != NULL )
		Line->m_SubNext->m_SubPrev = Line->m_SubPrev;
	else
		m_OldListLast = Line->m_SubPrev;

	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line->m_SubNext;
	else
		m_OldListFirst = Line->m_SubNext;

	Line->m_SubListType = LkSegmLine_NoList;
}

void LkSegmCNF::NewListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_SubNext != NULL )
		Line->m_SubNext->m_SubPrev = Line->m_SubPrev;
	else
		m_NewListLast = Line->m_SubPrev;

	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line->m_SubNext;
	else
		m_NewListFirst = Line->m_SubNext;

	Line->m_SubListType = LkSegmLine_NoList;
}

void LkSegmCNF::FreeListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_SubNext != NULL )
		Line->m_SubNext->m_SubPrev = Line->m_SubPrev;
	else
		m_FreeListLast = Line->m_SubPrev;

	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line->m_SubNext;
	else
		m_FreeListFirst = Line->m_SubNext;

	Line->m_SubListType = LkSegmLine_NoList;
}

void LkSegmCNF::SubListDelete( LkSegmLine *Line )
{
	// Detach
	if ( Line->m_SubNext != NULL )
		Line->m_SubNext->m_SubPrev = Line->m_SubPrev;
	else // Update last element depending on the list type
		switch( Line->m_SubListType )
		{
		case LkSegmLine_StabList :
			m_StabListLast = Line->m_SubPrev; break;
		case LkSegmLine_OldList :
			m_OldListLast = Line->m_SubPrev; break;
		case LkSegmLine_NewList :
			m_NewListLast = Line->m_SubPrev; break;
		case LkSegmLine_FreeList :
			m_FreeListLast = Line->m_SubPrev; break;
#ifdef _DEBUG
		default :
			MessageBox(0,"ERROR: Cannot determine sublist type","Debug Message",MB_OK); break;
#endif
		}

	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line->m_SubNext;
	else // Update first element depending on the list type
		switch( Line->m_SubListType )
		{
		case LkSegmLine_StabList :
			m_StabListFirst = Line->m_SubPrev; break;
		case LkSegmLine_OldList :
			m_OldListFirst = Line->m_SubPrev; break;
		case LkSegmLine_NewList :
			m_NewListFirst = Line->m_SubPrev; break;
		case LkSegmLine_FreeList :
			m_FreeListFirst = Line->m_SubPrev; break;
#ifdef _DEBUG
		default :
			MessageBox(0,"ERROR: Cannot determine sublist type","Debug Message",MB_OK); break;
#endif
		}

	Line->m_SubListType = LkSegmLine_NoList;
}

void LkSegmCNF::InfoListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_InfoNext = Line1;
	Line->m_InfoPrev = Line1->m_InfoPrev;

	// Attach two others
	Line1->m_InfoPrev = Line;
	if ( Line->m_InfoPrev != NULL )
		Line->m_InfoPrev->m_InfoNext = Line;
	else
		m_InfoListFirst = Line;
}

void LkSegmCNF::CompSumListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_CompSumNext = Line1;
	Line->m_CompSumPrev = Line1->m_CompSumPrev;

	// Attach two others
	Line1->m_CompSumPrev = Line;
	if ( Line->m_CompSumPrev != NULL )
		Line->m_CompSumPrev->m_CompSumNext = Line;
	else
		m_CompSumListFirst = Line;
}

void LkSegmCNF::StabListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_SubNext = Line1;
	Line->m_SubPrev = Line1->m_SubPrev;

	// Attach two others
	Line1->m_SubPrev = Line;
	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line;
	else
		m_StabListFirst = Line;

	Line->m_SubListType = LkSegmLine_StabList;
}

void LkSegmCNF::OldListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_SubNext = Line1;
	Line->m_SubPrev = Line1->m_SubPrev;

	// Attach two others
	Line1->m_SubPrev = Line;
	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line;
	else
		m_OldListFirst = Line;

	Line->m_SubListType = LkSegmLine_OldList;
}

void LkSegmCNF::NewListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_SubNext = Line1;
	Line->m_SubPrev = Line1->m_SubPrev;

	// Attach two others
	Line1->m_SubPrev = Line;
	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line;
	else
		m_NewListFirst = Line;

	Line->m_SubListType = LkSegmLine_NewList;
}

void LkSegmCNF::FreeListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_SubNext = Line1;
	Line->m_SubPrev = Line1->m_SubPrev;

	// Attach two others
	Line1->m_SubPrev = Line;
	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line;
	else
		m_FreeListFirst = Line;

	Line->m_SubListType = LkSegmLine_FreeList;
}

void LkSegmCNF::SubListInsert( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Insert Line before Line1

	// Attach itself
	Line->m_SubNext = Line1;
	Line->m_SubPrev = Line1->m_SubPrev;

	// Attach two others
	Line1->m_SubPrev = Line;
	if ( Line->m_SubPrev != NULL )
		Line->m_SubPrev->m_SubNext = Line;
	else
		switch( Line->m_SubListType )
		{
		case LkSegmLine_StabList :
			m_StabListFirst = Line; break;
		case LkSegmLine_OldList :
			m_OldListFirst = Line; break;
		case LkSegmLine_NewList :
			m_NewListFirst = Line; break;
		case LkSegmLine_FreeList :
			m_FreeListFirst = Line; break;
#ifdef _DEBUG
		default :
			MessageBox(0,"ERROR: Cannot determine sublist type","Debug Message",MB_OK); break;
#endif
		}

	Line->m_SubListType = Line1->m_SubListType;
}

LkSegmLine* LkSegmCNF::InfoListFind( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Find insertion position for Line after Line1 in the ordered list (NULL if appended)
	for ( Line1 = Line1->m_InfoNext; Line1 != NULL; Line1 = Line1->m_InfoNext )
		if ( Line1->m_Info > Line->m_Info ) // Primary order
			break;
		else if ( Line1->m_Info == Line->m_Info && Line1->m_CompSum > Line->m_CompSum ) // Secondary order
			break;

	return Line1;
}

LkSegmLine* LkSegmCNF::CompSumListFind( LkSegmLine *Line, LkSegmLine *Line1 )
{
	// Find insertion position for Line after Line1 in the ordered list (NULL if appended)
	for ( Line1 = Line1->m_CompSumNext; Line1 != NULL; Line1 = Line1->m_CompSumNext )
		if ( Line1->m_CompSum > Line->m_CompSum ) // Primary order
			break;
		else if ( Line1->m_CompSum == Line->m_CompSum && Line1->m_Info > Line->m_Info ) // Secondary order
			break;

	return Line1;
}

LkSegmLine* LkSegmCNF::NewListFind( LkSegmLine *Line )
{
	LkSegmLine *Line1;
	// Find insertion position for Line in the ordered list (NULL if appended)
	for ( Line1 = m_NewListLast; Line1 != NULL; Line1 = Line1->m_SubPrev )
		if ( Line1->m_CompSum <= Line->m_CompSum ) // Primary order
			break;
		// Secondary order -- we do not need it

	if ( Line1 != NULL )
		if ( Line1 != m_NewListLast )
			return Line1->m_SubNext;
		else
			return NULL;
	else
		if ( Line1 != m_NewListLast )
			return m_NewListFirst;
		else
			return NULL;
}

BOOL LkSegmCNF::InfoListIsOut( LkCompFloat LineInfo, LkCompLong LineCompSum )
{
	if ( m_NumLines < m_MaxLines )
		return FALSE;

	if ( m_InfoListLast->m_Info < LineInfo )
		return TRUE;

	if ( m_InfoListLast->m_Info == LineInfo )
		if ( m_InfoListLast->m_CompSum <= LineCompSum )
			return TRUE;

	return FALSE;
}

void LkSegmCNF::CopyLine( LkSegmLine *LineSrc, LkSegmLine *LineDst )
{
	LkCoord var;

	// Semantics
	CopyMemory( LineDst->m_Comps, LineSrc->m_Comps, m_LineWidthB*sizeof(LkSegm) );

	// Some fields of the line header
	LineDst->m_Length = LineSrc->m_Length;
	LineDst->m_TotLength = LineSrc->m_TotLength;

	LineDst->m_Const = LineSrc->m_Const;
	LineDst->m_CompSum = LineSrc->m_CompSum;
	LineDst->m_Info = LineSrc->m_Info;

	LineDst->m_Flags = LineSrc->m_Flags;

	for ( var = 0; var < m_Width; var++ )
	{
		// Some fields of all section headers
		LineDst->m_Sects[var].m_Length = LineSrc->m_Sects[var].m_Length;
		LineDst->m_Sects[var].m_CompSum = LineSrc->m_Sects[var].m_CompSum;
	}
}

void LkSegmCNF::ReduceLine( LkSegmLine *Line, LkCompByte Constant )
{

}

BOOL LkSegmCNF::IsAbsorbedByStab( LkSegmLine *Line )
{
	LkCoord var, segm;

	LkSegmLine *Line1;

	LkSegmSect *Sect;
	LkSegmSect *Sect1;

	LkSegm *Comp;
	LkSegm *Comp1;

	// Try to absorb by stable lines
	for ( Line1 = m_StabListFirst; Line1 != NULL; Line1 = Line1->m_SubNext )
	{
		if ( Line1->m_CompSum > Line->m_CompSum )
			break;

#ifdef _DIAG
	m_DiagAbsorbByStab++;
#endif // _DIAG

		// Check critical pair in the case the constant has not changed (always in crisp case)
		if ( (Line1->m_Comps[Line->m_OldCompOffs] & m_Syn[Line->m_OldVar].m_PointSegm) == 0 )
		{
#ifdef _DIAG
			m_DiagAbsorbByStab_Critical++;
#endif // _DIAG
			continue;
		}

		for ( var = 0, Sect1 = Line1->m_Sects, Sect = Line->m_Sects; var < m_Width; var++, Sect1++, Sect++ )
			if ( Sect1->m_CompSum > Sect->m_CompSum )
				break;
		if ( var != m_Width )
		{
#ifdef _DIAG
			m_DiagAbsorbByStab_Sects++;
#endif // _DIAG
			continue;
		}

		for ( segm = 0, Comp1 = Line1->m_Comps, Comp = Line->m_Comps; segm < m_LineWidthB; segm++ )
			if ( (Comp1[segm] & Comp[segm]) != Comp1[segm] )
				break;
		if ( segm != m_LineWidthB )
		{
#ifdef _DIAG
			m_DiagAbsorbByStab_Comps++;
#endif // _DIAG
			continue;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL LkSegmCNF::IsAbsorbedByNew( LkSegmLine *Line )
{
	return FALSE; // Children (new lines) cannot absorb each other
}

BOOL LkSegmCNF::IsLineAbsorbedNew( LkSegmLine *Line1, LkSegmLine *Line2 )
{
	return FALSE; // Children (new lines) cannot absorb each other
}
