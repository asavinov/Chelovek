//================================================================
// RiData
//================================================================
// This object provides an access to records. It hides concrete 
// format of data and data engine, i.e., we have only records.
class RiData: public RiNode {
public:

  RiData( char* NewName );
  ~RiData();

  virtual RiError Connect( LPCTSTR NewDBName ) = 0;
  virtual RiError Disconnect() = 0;

  virtual RiError Open( LPCTSTR NewTable, CStringArray* NewFields ) = 0;
  virtual RiError Close() = 0;
  };



//================================================================
// RiData_Dao
//================================================================
// This object allows us to access files though DAO
class RiData_Dao: public RiData {
public:

  CString DatabaseName;
  CString ConnectString;
  CString QueryString;

  CDaoWorkspace* Workspace;
  CDaoDatabase* Database;
  CDaoRecordset* Recordset; 


  RiData_Dao( char* NewName );
  ~RiData_Dao();

  virtual RiError Connect( LPCTSTR NewDBName );
  virtual RiError Disconnect();

  virtual RiError Open( LPCTSTR NewTable, CStringArray* NewFields );
  virtual RiError Close();

  };

