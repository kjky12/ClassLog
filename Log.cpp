/********************************************************************
	created:	2005/03/02
	created:	2:3:2005   13:29
	filename: 	Log.cpp
	file path:	Updater
	file base:	Log
	file ext:	cpp
	author:		Geert van Horrik
	
	purpose:	
*********************************************************************/

//*********************************************************************
// INCLUDES
//*********************************************************************

#include "stdafx.h"
#include "Log.h"
#include <io.h>		//access

//*********************************************************************
// INITIALISATION
//*********************************************************************

std::auto_ptr<CLog> CLog::sm_inst;

//*********************************************************************
// CONSTRUCTOR & DESTRUCTOR
//*********************************************************************

CLog::CLog()
{
	// Set file handler to NULL
	m_pLogFile = NULL;

	m_bAutoDirectory = false;

	m_strAutoDirectoryPath = L"";
}

//*********************************************************************
// PUBLIC FUNCTIONS
//*********************************************************************

CLog * CLog::Instance()
{
	if(sm_inst.get() == 0)
	sm_inst = auto_ptr<CLog>(new CLog);
	return sm_inst.get();

	/* FOLLOWING CODE WORKS ONLY IN VC7
	if(sm_inst.get() == 0)
	sm_inst.reset(new CLog);
	return sm_inst.get();
	*/
}

//=====================================================================

void CLog::CreateLog(CString sFilename, bool bEnableTimestamp)
{
	BOOL bCheck;

	char chPath[MAX_PATH] = {NULL,};
	strcpy_s(chPath, CT2A(sFilename));
	bCheck = _access((const char*)chPath, 0);
	
	if(bCheck == -1)
	{
		m_pLogFile = new CFile(sFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);

	}
	else
	{
		m_pLogFile = new CFile(sFilename, CFile::modeWrite | CFile::shareDenyWrite);
	}
	
	// Set timestamp option
	m_bEnableTimestamp = bEnableTimestamp;

	CTime tmCurr = CTime::GetCurrentTime();
	tmMakeLogTime = tmCurr;

	// Write first part so the file is unicode
	TCHAR bom = (TCHAR)0xFEFF;
	m_pLogFile->Write(&bom, sizeof(TCHAR));
}

//=====================================================================

void CLog::Log(CString sText)
{
	if(m_bAutoDirectory)
	{
		CTime tmCurr = CTime::GetCurrentTime();
		if(tmCurr.GetYear() != tmMakeLogTime.GetYear()  ||tmCurr.GetMonth() != tmMakeLogTime.GetMonth() ||tmCurr.GetDay() != tmMakeLogTime.GetDay())
		{
			CTime tmCurr = CTime::GetCurrentTime();
			CString strFileName = L"";
			strFileName.Format(L"%04d%02d%02d.txt", tmCurr.GetYear(), tmCurr.GetMonth(), tmCurr.GetDay());

// 			CFileFind	pFindFile;
// 			if(!pFindFile.FindFile(m_strAutoDirectoryPath + strFileName))
// 			{
// 
// 			}
			if (m_pLogFile)
				CloseLog();

			CreateLog(m_strAutoDirectoryPath+ strFileName, m_bEnableTimestamp);


		}
	}
	


	// Check if log is already created
	if (m_pLogFile == NULL)
		return;


	m_pLogFile->SeekToEnd();
	// Should we add time to beginning of string?
	if (m_bEnableTimestamp)
	{
		sText.Insert(0, GetTime());
	}

	// Add \n\r to end of text
	sText.Insert(sText.GetLength(), _T("\r\n"));

	// Log text
	m_pLogFile->Write(sText, sText.GetLength() * 2);

	m_pLogFile->Flush();
}

//=====================================================================

void CLog::CloseLog()
{
	// Close file
	m_pLogFile->Close();

	// Delete object
	delete m_pLogFile;

	// Set handle to NULL
	m_pLogFile = NULL;
}

//=====================================================================

void CLog::EnableTimestamp(bool bEnable)
{
	// Set timestamp option
	m_bEnableTimestamp = bEnable;
}

//*********************************************************************
// PRIVATE FUNCTIONS
//*********************************************************************

CString CLog::GetTime()
{
	// Declare variables
	SYSTEMTIME time;
	CString sTime;

	// Get time
	GetLocalTime(&time);

	// Set up time
	sTime.Format(_T("[%02d:%02d:%02d:%03d] - "), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	// Return value
	return sTime;
}

void CLog::SetAutoDirectory(bool bFlag)
{
	m_bAutoDirectory = bFlag;
}

void CLog::SetAutoDirectoryName(CString strName)
{
	if(strName.Right(0) != L"\\")
		strName += L"\\";
	
	m_strAutoDirectoryPath = strName;

	CreateDirectory(m_strAutoDirectoryPath, NULL);

	

}