// RiDataSourceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiDataSourceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiDataSourceDlg dialog


CRiDataSourceDlg::CRiDataSourceDlg( CRiDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRiDataSourceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiDataSourceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Document = pDoc; // Save the document with this data source
	DataSource = NULL; // No data source object
}


void CRiDataSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiDataSourceDlg)
	DDX_Control(pDX, IDC_DS_FIELDS, m_Fields);
	DDX_Control(pDX, IDC_DS_TABLES, m_Tables);
	DDX_Control(pDX, IDC_DS_DBSELECT, m_DBSelect);
	DDX_Control(pDX, IDC_DS_DATABASE, m_Database);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiDataSourceDlg, CDialog)
	//{{AFX_MSG_MAP(CRiDataSourceDlg)
	ON_BN_CLICKED(IDC_DS_DBSELECT, OnDsDbselect)
	ON_LBN_SELCHANGE(IDC_DS_TABLES, OnSelchangeDsTables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiDataSourceDlg message handlers

void CRiDataSourceDlg::OnDsDbselect() 
{
	// TODO: Add your control notification handler code here
	
	//
	// Request database name
	//
	CFileDialog FileDialog
		( 
		TRUE, // FileOpen instead of FileSaveAs
		"txt", // Default file extension to be appended if not indicated
		NULL, // initial file name to be show at the start
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // Flags
		"CSV Files (*.csv; *.txt; *.dat)|*.csv;*.txt;*.dat|All Files (*.*)|*.*||", 
		NULL // Parent window
		);

	int ret = FileDialog.DoModal();
	if ( ret != IDOK )
		return; // Canceled. Nothing has changed

	KbDataSourceCSV *Data = new KbDataSourceCSV( "Data Source" );

	KbError err;

	err = Data->SetDatabaseName( FileDialog.GetPathName(), TRUE );
	if ( err != KbError_Success )
	{
		// ERROR: Cannot create data source
		delete Data;
		return;
	}

	err = Data->Connect();
	if ( err != KbError_Success )
	{
		// ERROR: Cannot open the database
		Data->Disconnect();
		delete Data;
		return;
	}

	// Now remember this new database, i.e., insert new datasource instead of the 
	if ( DataSource != NULL )
	{
		DataSource->Disconnect();
		delete DataSource;
	}

	DataSource = Data;

	// Write database name to the editbox IDC_DS_DATABASE (member variabel m_Database)
	m_Database.SetWindowText( FileDialog.GetPathName() );

	// Insert the table name into the list of tables
	m_Tables.ResetContent();

	int index = m_Tables.AddString( FileDialog.GetFileName() );

	m_Tables.SetCurSel( index );

	// The previous command does not generate ON_SEL_CHANGE message and the fields are not retrieved
	// so call this function explicitely
	OnSelchangeDsTables();
}

void CRiDataSourceDlg::OnSelchangeDsTables() 
{
	// TODO: Add your control notification handler code here
	
	//
	// When the table has changed we have to retrieve all its fields and add them to the list
	//

	if ( DataSource == NULL )
	{
		return; // No datasource has been chosen
	}

	int CurSel = m_Tables.GetCurSel();
	if ( CurSel == -1 || CurSel == LB_ERR )
	{
		return; // ERROR:
	}

	m_Fields.ResetContent();

	char FieldName[2048];
	LkCoord FieldNum = DataSource->GetAbsFieldCount();
	for( LkCoord i = 0; i < FieldNum; i++ )
	{
		DataSource->GetAbsFieldName( i, FieldName );
		m_Fields.AddString( FieldName );
	}

	// Set initial selection (all fields)
	m_Fields.SelItemRange( TRUE, 0, FieldNum-1 );
}

void CRiDataSourceDlg::OnOK() 
{
	// TODO: Add extra validation here

	if ( DataSource == NULL )
	{
		return; // ERROR: Data source has not been chosen
	}

	//
	// Check if the recordset can be created
	//

	int CurSel = m_Tables.GetCurSel();
	if ( CurSel == -1 || CurSel == LB_ERR )
	{
		return; // ERROR:
	}

	// Loop on all selected listbox items
	CString FieldName;
	CStringArray Fields;

	for ( int i = 0; i < m_Fields.GetCount(); i++ )
	{
		if ( LB_ERR == m_Fields.GetSel( i ) )
			continue;

		if ( 0 == m_Fields.GetSel( i ) )
			continue;

		// Insert the selected item into the collection
		m_Fields.GetText( i, FieldName );
		Fields.Add( FieldName );
	}

	KbError err;

	err = DataSource->SetFields( & Fields );
	if ( err != KbError_Success )
	{
		return; // ERROR: Cannot create datasource (recordset)
	}

	err = DataSource->Open();
	if ( err != KbError_Success )
	{
		return; // ERROR: Cannot create datasource (recordset)
	}

	DataSource->Close();
	DataSource->Disconnect();

	//
	// Now transfer this data source as an object to the document (and delete old one)
	//

	if ( Document->KB->DataSource != NULL )
	{
		delete Document->KB->DataSource;
		Document->KB->DataSource = NULL;
	}

	Document->KB->DataSource = DataSource;

	DataSource = NULL; // Forget about it

	//
	// Change the document state
	//
	Document->SetModifiedFlag( TRUE );
	Document->UpdateAllViews( NULL, 0L, NULL );

	CDialog::OnOK();
}

void CRiDataSourceDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	//
	// If data source has been created, delete it
	//

	if ( DataSource != NULL )
	{
		delete DataSource;
		DataSource = NULL;
	}

	CDialog::OnCancel();
}
