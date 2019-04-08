#ifndef __EXCEPTIONREPORT_H__
#define __EXCEPTIONREPORT_H__


#ifdef EXCEPTIONREPORT_EXPORTS

#define EXCEPTIONREPORT_API __declspec(dllexport)

#else

#define EXCEPTIONREPORT_API __declspec(dllimport)
#pragma comment(lib,"ExceptionReport.lib")

#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dbghelp.h>

enum BasicType
{
   btNoType = 0,
   btVoid = 1,
   btChar = 2,
   btWChar = 3,
   btInt = 6,
   btUInt = 7,
   btFloat = 8,
   btBCD = 9,
   btBool = 10,
   btLong = 13,
   btULong = 14,
   btCurrency = 25,
   btDate = 26,
   btVariant = 27,
   btComplex = 28,
   btBit = 29,
   btBSTR = 30,
   btHresult = 31
};

// This class is exported from the ExceptionReport.dll
class EXCEPTIONREPORT_API CExceptionReport {
public:
	static int __cdecl _tprintf(const TCHAR * format, ...);
	static BasicType GetBasicType( DWORD typeIndex, DWORD64 modBase );
	static char *FormatOutputValue(char * pszCurrBuffer,BasicType basicType,DWORD64 length,PVOID pAddress );
	static char *DumpTypeIndex(char * pszCurrBuffer,DWORD64 modBase,DWORD dwTypeIndex,unsigned nestingLevel,DWORD_PTR offset,bool & bHandled );
	static bool FormatSymbolValue(PSYMBOL_INFO pSym,STACKFRAME * sf,char * pszBuffer,unsigned cbBuffer );
	static BOOL CALLBACK EnumerateSymbolsCallback(PSYMBOL_INFO pSymInfo,ULONG SymbolSize,PVOID UserContext );
	static void WriteStackDetails( PCONTEXT pContext,bool bWriteVariables );
	static BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset );
	static LPTSTR GetExceptionString( DWORD dwCode );
	static void GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );
	static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo );
	static void SetLogFileName( PTSTR pszLogFileName );
	static void SetErrorExcuteFile( LPCTSTR pszCommandLine ) {lstrcpyn(m_szCommendLine,pszCommandLine,MAX_PATH*2);}
	LPCTSTR GetLogFileName(){return m_szLogFileName;}
	CExceptionReport();
	virtual ~CExceptionReport();
	static bool m_bShowMessageBox;
	static bool m_bUpPassException;
protected:	
	static TCHAR m_szLogFileName[MAX_PATH];
	static TCHAR m_szCommendLine[MAX_PATH*2];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
	static HANDLE m_hReportFile;
	static HANDLE m_hProcess;
};

// extern EXCEPTIONREPORT_API CExceptionReport g_ExceptionReport;
#ifdef EXCEPTIONREPORT_EXPORTS
#else
static CExceptionReport m_gExceptionRepot;
#pragma message("----------------------------------->Add Exception Report To this project")
#endif


#endif //__EXCEPTIONREPORT_H__