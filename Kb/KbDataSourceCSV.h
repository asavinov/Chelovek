#if !defined(__KbDataSourceCSV_h__)
#define __KbDataSourceCSV_h__

/*
This object allows us to access data directly in text files.
When creating it is necessary to provide the object name which 
identifies it in the system. This name is used exclusively for 
identification purposes and is mainly used to show the object 
to the user. It is not used when accessing real file, i.e., it 
is not connected with data structure.

Only the database name and the list of fields are persistent.
*/

#define KbDataSourceCSV_BufSize 8192

class KbDataSourceCSV : public LkNode
{
private:

	CString m_DatabaseName; // Path to the data file including file name
	CString m_TableName; // Data file name

	BOOL m_FieldNames;

	CStringArray m_Fields; // Names of fields to query
	CDWordArray m_Indeces; // Indeces of fields to query

	HFILE m_File;
	long m_FileSize;

	char *m_Buffer;
	long m_BufPosition; // Buffer offset in file
	long m_Position; // Current position in buffer

public:

	// Construction/Destruction
	KbDataSourceCSV( char* Name );
	~KbDataSourceCSV();

	// Connect/disconnect
	KbError SetDatabaseName( CString DatabaseName, BOOL FieldNames = TRUE );
	KbError Connect(); // Connect to the datasource
	KbError Disconnect(); // Disconnect from the database
	BOOL IsConnected();

	// Open/Close
	KbError SetFields( CStringArray *Fields ); // Set the list of fields
	KbError Open(); // Open recordset
	KbError Close(); // Close recordset
	BOOL IsOpen();

	// Movement
	KbError MoveFirst(); // Move to the first record in the table
	KbError MoveNext(); // Move to the next record in the table

	// Get values
	LkCoord GetAbsFieldCount();
	LkCoord GetFieldCount();

	KbError GetAbsValue( LkCoord FieldNum, char *Value );
	KbError GetValue( LkCoord FieldNum, char *Value );

	int GetAbsFieldName( LkCoord FieldNum, char *Name );
	int GetFieldName( LkCoord FieldNum, char *Name );

	unsigned char GetAbsFieldType( LkCoord FieldNum );
	unsigned char GetFieldType( LkCoord FieldNum );

protected:

	unsigned char CalcValueType( char *Value );
	KbError MoveVeryFirst(); // Move to the very first record in the table (with names of variables)

};

#define KbField_Null    0
#define KbField_Unknown 1
#define KbField_String  2
#define KbField_Number  3

class KbField : public LkNode
{
private:
	unsigned char m_Type;

public:
	KbField( char* Name, unsigned char Type = KbField_Unknown );
	~KbField();

	unsigned char GetType() { return m_Type; };
	void SetType( unsigned char Type ) { m_Type = Type; };

};

#endif // !defined(__KbDataSourceCSV_h__)
