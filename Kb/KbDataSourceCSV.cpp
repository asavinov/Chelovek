#include "stdafx.h"
#include "..\Lk\Lk.h"
#include "Kb.h"

//
// Construction/Destruction
//

KbField::KbField( char* Name, unsigned char Type )
	: LkNode( Name )
{
	m_Type = Type;
}

KbField::~KbField()
{

}

//
// Construction/Destruction
//

KbDataSourceCSV::KbDataSourceCSV( char* Name )
	: LkNode( Name )
{
	m_DatabaseName.Empty();
	m_TableName.Empty();

	m_FieldNames = TRUE;
}

KbDataSourceCSV::~KbDataSourceCSV()
{
	Close();
	Disconnect();

	m_TableName.Empty();
	m_DatabaseName.Empty();

	m_Fields.RemoveAll();
	m_Indeces.RemoveAll();
}

//
// Connect/disconnect
//

KbError KbDataSourceCSV::SetDatabaseName( CString DatabaseName, BOOL FieldNames )
{
	if ( lstrcmpi(m_DatabaseName, DatabaseName) == 0 )
		return KbError_Success; 

	// If not the same database, close and disconnect
	Close();
	Disconnect();

	m_DatabaseName = DatabaseName; // Copy string

	m_FieldNames = FieldNames;

	return KbError_Success; 
}

KbError KbDataSourceCSV::Connect()
{
	// In connected state the file is open while buffer is closed
	// Build table structure and determine field types
	Close();
	Disconnect();

	// Allocate file buffer
	m_Buffer = (char*) GlobalAlloc ( GPTR, KbDataSourceCSV_BufSize );
	if ( m_Buffer == NULL ) 
		return KbError_Memory; // ERROR: Memory

	// Open file
	m_File = _lopen ( m_DatabaseName, OF_READ );
	if ( m_File == -1 )
	{
		Disconnect();
		return KbError_OpenFile; // ERROR: Cannot open file
	}

	m_FileSize = GetFileSize( (HANDLE) m_File, NULL );

	_hread( m_File, m_Buffer, min(KbDataSourceCSV_BufSize, m_FileSize) );

	m_BufPosition = 0;
	m_Position = 0;

	KbField *Field;

	// Delete all current fields
	while ( N() )
	{
		Field = (KbField*) Coord2Object( N() - 1 );
		Exclude( Field );
		delete Field;
	}

	// Determine the count and names of fields
	LkCoord field;
	char NameBuf[2048];
	char ValBuf[2048];

	for ( field = 0; ; field++ )
	{
		GetAbsValue( field, NameBuf );
		if ( lstrlen(NameBuf) == 0 )
			break;

		if ( m_FieldNames )
		{
			MoveNext();
			GetAbsValue( field, ValBuf );
			MoveVeryFirst();
		}
		else
		{
			lstrcpy( ValBuf, NameBuf );
			wsprintf( NameBuf, "Field %i", field + 1 );
		}

		Field = new KbField( NameBuf, CalcValueType( ValBuf ) );

		Include( Field );
	}
              
	if ( N() == 0 )
	{
		// Clear
		Disconnect();
		return KbError_General; // Bad field count
	}

	// If connected the buffer is closed while file remains open
	if ( m_Buffer != NULL )
		GlobalFree( m_Buffer );

	m_Buffer = NULL;

	m_BufPosition = 0;
	m_Position = 0;

	return KbError_Success; 
}

KbError KbDataSourceCSV::Disconnect()
{
	// Close recordset
	Close();

	KbField *Field;

	// Delete all current fields
	while ( N() )
	{
		Field = (KbField*) Coord2Object( N() - 1 );
		Exclude( Field );
		delete Field;
	}

	// Close file
	_lclose ( m_File );
	m_File = 0;

	return KbError_Success; 
}

BOOL KbDataSourceCSV::IsConnected()
{
	if ( m_File == 0 )
		return FALSE;

	return TRUE;
}

//
// Open/Close
//

KbError KbDataSourceCSV::SetFields( CStringArray *Fields )
{
	LkCoord field;

	if ( Fields == NULL )
	{
		m_Fields.RemoveAll();
		m_Indeces.RemoveAll();
		return KbError_Success; 
	}

	if ( Fields->GetSize() == 0 )
		return KbError_BadSize; 

	m_Fields.Copy( *Fields );

	m_Indeces.SetSize( m_Fields.GetSize() );

	CString Name;
	LkCoord Index;

	// For each field name determine its index
	for( field = 0; field < (LkCoord) m_Fields.GetSize(); field++ )
	{
		Name = m_Fields.GetAt( field );

		Index = Name2Coord( (char*) (LPCSTR) Name );
		if ( Index >= N() )
		{
			m_Fields.RemoveAll();
			m_Indeces.RemoveAll();
			return KbError_BadIndex; // ERROR: No such field name in the list
		}

		m_Indeces.SetAt( field, Index );
	}

	return KbError_Success; 
}

KbError KbDataSourceCSV::Open()
{
	// Allocate file buffer
	m_Buffer = (char*) GlobalAlloc ( GPTR, KbDataSourceCSV_BufSize );
	if ( m_Buffer == NULL ) 
		return KbError_Memory; // ERROR: Memory

	MoveFirst();

	return KbError_Success;
}

KbError KbDataSourceCSV::Close()
{
	// In connected state the buffer is closed while file remains open
	if ( m_Buffer != NULL )
		GlobalFree( m_Buffer );

	m_Buffer = NULL;

	m_BufPosition = 0;
	m_Position = 0;

	return KbError_Success; 
}

BOOL KbDataSourceCSV::IsOpen()
{
	if ( m_Buffer == NULL )
		return FALSE;

	return TRUE;
}

//
// Movement
//

KbError KbDataSourceCSV::MoveFirst()
{
	MoveVeryFirst();

	if ( m_FieldNames )
	{
		MoveNext();
	}

	return KbError_Success;
}

KbError KbDataSourceCSV::MoveNext()
{
	//
	// Move pointer to the start of the next record (after the end of the current)
	//

	for ( long NewPos = m_Position; m_Buffer[NewPos] != '\r' && m_Buffer[NewPos] != '\n'; NewPos++ )
	{
		// There may be 2 bad situations which can prevent us from moving the offset forward

		if ( (m_BufPosition + NewPos) == (m_FileSize - 1) )
		{
			return KbError_EOF; // Unexpected end of file -- no next record, no movement
		}

		if ( NewPos == (KbDataSourceCSV_BufSize - 1) )
		{
			// End of buffer
			// Load new portion from the file

			m_BufPosition += KbDataSourceCSV_BufSize - 1;
			NewPos = 0;
			_llseek( m_File, m_BufPosition, FILE_BEGIN ); 
			_hread( m_File, m_Buffer, min(KbDataSourceCSV_BufSize, m_FileSize - m_BufPosition) );
		}

	}
	// Here it means that a symbol of the end of record has been encountered

	// Pass over bad symbols at the start of new record
	while ( m_Buffer[NewPos] == '\r' || m_Buffer[NewPos] == '\n' )
	{
		// There may be 2 bad situations which can prevent us from moving the offset forward

		if ( (m_BufPosition + NewPos) == (m_FileSize - 1) )
		{
			return KbError_EOF; // Unexpected end of file -- no next record, no movement
		}

		if ( NewPos == (KbDataSourceCSV_BufSize - 1) )
		{
			// End of buffer
			// Load new portion from the file

			m_BufPosition += KbDataSourceCSV_BufSize - 1;
			NewPos = 0;
			_llseek( m_File, m_BufPosition, FILE_BEGIN ); 
			_hread( m_File, m_Buffer, min(KbDataSourceCSV_BufSize, m_FileSize - m_BufPosition) );
		}

		NewPos++;	
	}
	// Now we are at the start of new record

	//
	// Then check if the whole record is in the buffer
	//
	for ( long offs = NewPos; m_Buffer[offs] != '\r' && m_Buffer[offs] != '\n'; offs++ )
	{

		// There may be 2 bad situations which can prevent us from moving the offset forward

		if ( (m_BufPosition + offs) == (m_FileSize - 1) )
		{
			// End of file: it is last record and in principle it is possible that it contains not all fields
			// Or this record may me ended with EOF without new line symbol
			// We suppose that it is good record
			break; 
		}

		if ( offs == (KbDataSourceCSV_BufSize - 1) )
		{
			// End of buffer
			// Load new portion from the file starting from NewPos found above

			m_BufPosition += NewPos;
			NewPos = 0;
			offs = 0; // Start again
			_llseek( m_File, m_BufPosition, FILE_BEGIN ); 
			_hread( m_File, m_Buffer, min(KbDataSourceCSV_BufSize, m_FileSize - m_BufPosition) );
		}

	}
	// Here it means that a symbol of the end of new record has been encountered
	// i.e., our record has an end

	m_Position = NewPos;

	return KbError_Success;
}

//
// Get values
//

LkCoord KbDataSourceCSV::GetAbsFieldCount()
{
	if ( ! IsConnected() )
		return LkCoord_Null;

	return N();
}

LkCoord KbDataSourceCSV::GetFieldCount()
{
	return m_Fields.GetSize();
}

KbError KbDataSourceCSV::GetAbsValue( LkCoord FieldNum, char *Value )
{
	// Here FieldNum is the number of field in the table (absolute)

	//
	// Searching for FieldNum commas in the current record
	//
	LkCoord CommasNum = 0;
	long ValPos;
	for ( ValPos = m_Position; CommasNum < FieldNum; ValPos++ )
	{
		// There may be 2 bad situations which can prevent us from moving the offset forward
		if ( (m_BufPosition + ValPos) == (m_FileSize - 1) )
		{
			// End of file: it is last record and in principle it is possible that it contains not all fields
			// We suppose that it is bad record (i.e., each record must end with end of line)
			Value[0] = 0;
			return KbError_EOF; // Unexpected end of file
		}

		if ( ValPos == (KbDataSourceCSV_BufSize - 1) )
		{
			// End of buffer
			// We do not load new portion since if everything is ok, the whole record must in one buffer, hence something wrong
			Value[0] = 0;
			return KbError_EOB; // ERROR: Unexpected end of buffer -- Bad work of MoveNext/Prev
		}

		if ( m_Buffer[ValPos] == '\r' || m_Buffer[ValPos] == '\n' )
		{
			Value[0] = 0;
			return KbError_EOL; // ERROR: Unexpected end of line
		}

		if ( m_Buffer[ValPos] == ',' )
			CommasNum++;
	}

	CString ValStr = "";
	for ( int ValLen = 0; m_Buffer[ValPos+ValLen] != ',' && m_Buffer[ValPos+ValLen] != '\r' && m_Buffer[ValPos+ValLen] != '\n'; ValLen++ )
	{
		ValStr += m_Buffer[ValPos+ValLen]; // Add one new character of the value
		if ( (m_BufPosition + ValPos + ValLen) == (m_FileSize - 1) )
			break;
	}

	ValStr.TrimLeft();
	ValStr.TrimRight();

	lstrcpy( Value, ValStr );

	return KbError_Success;
}

KbError KbDataSourceCSV::GetValue( LkCoord FieldNum, char *Value )
{
	// Here FieldNum is the number of field in the recordset (relative)

	if ( m_Indeces.GetSize() == 0 )
	{
		return GetAbsValue( FieldNum, Value );
	}

	if ( FieldNum >= (LkCoord) m_Indeces.GetSize() )
		return KbError_BadIndex;

	return GetAbsValue( m_Indeces.GetAt( FieldNum ), Value );
}

int KbDataSourceCSV::GetAbsFieldName( LkCoord FieldNum, char *Name )
{
	KbField *Field = (KbField*) Coord2Object( FieldNum );

	if ( Field == NULL )
		return KbError_BadIndex;

	return Field->GetName( Name );
}

int KbDataSourceCSV::GetFieldName( LkCoord FieldNum, char *Name )
{
	if ( m_Indeces.GetSize() == 0 )
	{
		return GetAbsFieldName( FieldNum, Name );
	}

	if ( FieldNum >= (LkCoord) m_Indeces.GetSize() )
		return KbError_BadIndex;

	return GetAbsFieldName( m_Indeces.GetAt( FieldNum ), Name );
}

unsigned char KbDataSourceCSV::GetAbsFieldType( LkCoord FieldNum )
{
	KbField *Field = (KbField*) Coord2Object( FieldNum );

	if ( Field == NULL )
		return KbField_Null;

	return Field->GetType();
}

unsigned char KbDataSourceCSV::GetFieldType( LkCoord FieldNum )
{
	if ( m_Indeces.GetSize() == 0 )
	{
		return GetAbsFieldType( FieldNum );
	}

	if ( FieldNum >= (LkCoord) m_Indeces.GetSize() )
		return KbField_Null;

	return GetAbsFieldType( m_Indeces.GetAt( FieldNum ) );
}

//
// Protected
//

unsigned char KbDataSourceCSV::CalcValueType( char *Value )
{
	// Determine the type of string value

	return KbField_String;
}

KbError KbDataSourceCSV::MoveVeryFirst()
{

	m_Position = 0;
	m_BufPosition = 0;
	_llseek( m_File, m_BufPosition, FILE_BEGIN );
	_hread( m_File, m_Buffer, min(KbDataSourceCSV_BufSize, m_FileSize) );

	return KbError_Success;
}
