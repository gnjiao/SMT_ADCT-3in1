#include "stdafx.h"
#include "winrarClass.h"

CString winRarInstallPath; //= "C:\\Program Files\\WinRAR\\WinRAR.exe";
WinRARClass::WinRARClass()
{
}


WinRARClass::~WinRARClass()
{
}


void WinRARClass::UnpackFile(const CString & strFilePath)
{
	
	CString strDestPath; //Ŀ���ѹλ��
	//int pos = strFilePath.ReverseFind('.');
	strDestPath = strFilePath;
	strDestPath = strDestPath.Left(strDestPath.ReverseFind('.'));
	//strDestPath += "\\src\\";
	//ɾ��ͬ���ļ�
	/*if (::PathFileExists(strFilePath));
	{
		DeleteFile(strFilePath);
	}*/
	// ����ļ�
	//DeleteDirectories(strDestPath); 
	/*if (FALSE == ::CreateDirectory(strDestPath, NULL))
	{
		AfxMessageBox("������ѹ·��ʧ��");
		return;
	}*/
	if (!PathIsDirectory(strDestPath))
	{

		::CreateDirectory(strDestPath, NULL);

	}
	//x��ѹ  -ibck��ִ̨�� -o+����ڣ��򸲸� -inul������������ʾ  
	//ʹ�� -ibck����С����ϵͳ��������
	CString strCmd = "\"" + winRarInstallPath + "\" x -ibck -o+ -inul \"" + strFilePath + "\" \"" + strDestPath + "\"";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	BOOL bRet = CreateProcess(NULL, strCmd.GetBuffer(MAX_PATH), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	DWORD dwExit = 0;
	if (bRet)
	{
		//����ط������¸ú���Ϊ����״̬  
		WaitForSingleObject(pi.hProcess, INFINITE);
		::GetExitCodeProcess(pi.hProcess, &dwExit);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	HWND OrderHwnd = ::FindWindow("ADCT", "����ѡ��");
	SendMessage(OrderHwnd, WM_OrderDlgMessage, MessageUnpackSuccess, 0);
	return;
}

void WinRARClass::DeleteDirectories(CString csPath)
{
	CFileFind finder;
	CString tempPath;
	tempPath.Format("%s%s", csPath, "//*.*");
	BOOL bWork = finder.FindFile(tempPath);
	while (bWork)
	{
		bWork = finder.FindNextFile();
		if (!finder.IsDots())
		{
			if (finder.IsDirectory())
			{
				DeleteDirectories(finder.GetFilePath());
			}
			else
			{
				DeleteFile(finder.GetFilePath());
			}
		}
	}
	finder.Close();
	RemoveDirectory(csPath);
}

void WinRARClass::FindWinRARPath()
{
	HKEY hKEY32,hKEY64, hKEY;
	HKEY  hKeyRoot = HKEY_LOCAL_MACHINE;
	long ret32 = (::RegOpenKeyEx(hKeyRoot, "SOFTWARE\\WinRAR", 0, KEY_READ | KEY_WOW64_32KEY, &hKEY32));
	long ret64 = (::RegOpenKeyEx(hKeyRoot, "SOFTWARE\\WinRAR", 0, KEY_READ | KEY_WOW64_64KEY, &hKEY64));
	/*CString temp;
	temp.Format("%d", ret0);
	AfxMessageBox(temp);*/
	if (ret32== ERROR_SUCCESS)
	{
		hKEY = hKEY32;
		LPBYTE getValue1 = new BYTE[80];//�õ��ļ�ֵ
		DWORD keyType = REG_SZ;//������������
		DWORD DataLen = 80;//�������ݳ���
		CString strUser = _T("exe32");//Ҫ��ѯ�ļ�����
		long ret1 = ::RegQueryValueEx(hKEY, strUser, NULL, &keyType, getValue1, &DataLen);

		if (ret1 == ERROR_SUCCESS)
		{
			winRarInstallPath = (char*)getValue1;
		}
		else
		{
			AfxMessageBox("�����޷���ѯ��WINRAR��ע�����Ϣ");
		}
	}
	else if (ret64 == ERROR_SUCCESS)
	{
		hKEY = hKEY64;
		LPBYTE getValue2 = new BYTE[80];//�õ��ļ�ֵ
		DWORD keyType = REG_SZ;//������������
		DWORD DataLen = 80;//�������ݳ���
		CString strUser64 = _T("exe64");//Ҫ��ѯ�ļ�����
		long ret2 = ::RegQueryValueEx(hKEY, strUser64, NULL, &keyType, getValue2, &DataLen);
		if (ret2 == ERROR_SUCCESS)
		{
			winRarInstallPath = (char*)getValue2;
		}
		else
		{
			AfxMessageBox("�����޷���ѯ��WINRAR��ע�����Ϣ");
		}
	}
	else//����޷���hKEY,����ֹ�����ִ��
	{
		AfxMessageBox("�����޷����йص�hKEY");
		return;
	}
	
	
	
	
}