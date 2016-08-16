#include "stdafx.h"
#include "Ri.h"

//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiData                                   >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiData::RiData( char* NewName ) 
  : RiNode( NewName, RiOperation_And ) {
//----------------------------------------------------------------

  // Initialize members

  }

//================================================================
// Destructor
RiData::~RiData() {
//----------------------------------------------------------------

  // Destroy members

  }



//<<<<<<<<<<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>>>>>>>//
//<<<<<<<<<< RiData_Dao                               >>>>>>>>>>//
//<<<<<<<<<<__________________________________________>>>>>>>>>>//

//================================================================
// Constructor.
RiData_Dao::RiData_Dao( char* NewName ) 
    : RiData( NewName ) {
//----------------------------------------------------------------

  DatabaseName.Empty();
  ConnectString.Empty();
  QueryString.Empty();

  Workspace = NULL;
  Database = NULL; // No CDaoDatabase object
  Recordset = NULL; // No CDaoRecordset object

  }

//================================================================
// Destructor
RiData_Dao::~RiData_Dao() {
//----------------------------------------------------------------

  // Destroy members
  Disconnect();
  Close();

  DatabaseName.Empty();
  ConnectString.Empty();
  QueryString.Empty();

  }

//================================================================
// Connect to the datasource
RiError RiData_Dao::Connect( LPCTSTR NewDBName ) {
//----------------------------------------------------------------

  CDaoDatabase *NewDB;
  CDaoWorkspace *NewWS;

  if ( NewDBName == NULL ) {

    // Try to connect with old parameters 

    if ( Database != NULL ) {

      if ( Database->IsOpen() ) {
        return RiError_Success; // Already opened with the current parameters
        }
      else {
        NewDB = Database; // Use old CDatabase object which is not opemed for unknown reason (something wrong)
        Database == NULL;
        }

      }
    else {
      NewWS = new CDaoWorkspace();

      NewWS->Create( "Workspace", "admin", "" );

      NewDB = new CDaoDatabase( NewWS );
      }

    NewDB->Open( DatabaseName, FALSE, TRUE, ConnectString );

    }
  else {

    // Try to connect with new parameters

    NewWS = new CDaoWorkspace();

    NewWS->Create( "Workspace", "admin", "" );

    NewDB = new CDaoDatabase( NewWS );

    NewDB->Open( NewDBName, FALSE, TRUE, "Text;" );

    //
    // Save new connection parameters for future use
    //
    DatabaseName = NewDB->GetName();
    ConnectString = NewDB->GetConnect();
    }

  Disconnect(); // Disconnect from the current source
  Workspace = NewWS;
  Database = NewDB;

  return RiError_Success; 
  }

//================================================================
// Disconnect from the database
RiError RiData_Dao::Disconnect() {
//----------------------------------------------------------------

  //
  // Close recordset
  //
  Close();

  //
  // Close database
  //
  if ( Database == NULL ) 
    return RiError_Success; // Already disconnected (database is closed)

  if ( Database->IsOpen() ) 
    Database->Close();

  delete Database;

  Database = NULL;

  //
  // Close workspace
  //
  if ( Workspace == NULL ) 
    return RiError_Success;

  if ( Workspace->IsOpen() ) 
    Workspace->Close();

  delete Workspace;

  Workspace = NULL;

  return RiError_Success; 
  }

//================================================================
// Open recordset by running query
RiError RiData_Dao::Open( LPCTSTR NewTable, CStringArray* NewFields ) {
//----------------------------------------------------------------

  RiError ret;

  //
  // Try to connect with default parameters if not connected
  //
  if ( Database == NULL ) {
    ret = Connect( NULL );
    if ( ret != RiError_Success )
      return ret;
    }
  else if ( ! Database->IsOpen() ) {
    ret = Connect( NULL );
    if ( ret != RiError_Success )
      return ret;
    }

  CDaoRecordset *NewRS;

  if ( NewTable == NULL ) {

    // Try to open with old parameters

    NewRS = new CDaoRecordset(Database);

    NewRS->Open( dbOpenSnapshot, QueryString, dbReadOnly );

    }
  else {

    // Try to open with new parameters

    //
    // Build SQL string
    //
    int i;
    CString StrSQL = "SELECT ";

    if ( NewFields == NULL ) {
      StrSQL += "*";
      }
    else {

      for ( i = 0; i < NewFields->GetSize(); i++ ) {
        StrSQL += NewFields->GetAt(i);
        StrSQL += ", ";
        }
      StrSQL = StrSQL.Left( StrSQL.GetLength() - 2 );

      }

    StrSQL += " FROM [";
    StrSQL += NewTable;
    StrSQL += "]";

    NewRS = new CDaoRecordset(Database);

    NewRS->Open( dbOpenSnapshot, StrSQL, dbReadOnly );

    //
    // Save new query parameters for future use
    //
    QueryString = NewRS->GetSQL();
    }

  Close(); // Close the current recordset (if any) before attaching new one
  Recordset = NewRS;

  return RiError_Success; 
  }

//================================================================
// Disconnect from the database
RiError RiData_Dao::Close() {
//----------------------------------------------------------------

  //
  // Close recordset
  //
  if ( Recordset == NULL ) 
    return RiError_Success; // Already closed (no recordset)

  if ( Recordset->IsOpen() ) 
    Recordset->Close();

  delete Recordset;

  Recordset = NULL;

  return RiError_Success; 
  }

