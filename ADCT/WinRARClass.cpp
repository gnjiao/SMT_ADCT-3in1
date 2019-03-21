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
	
	CString strDestPath; //目标解压位置
	//int pos = strFilePath.ReverseFind('.');
	strDestPath = strFilePath;
	strDestPath = strDestPath.Left(strDestPath.ReverseFind('.'));
	//strDestPath += "\\src\\";
	//删除同名文件
	/*if (::PathFileExists(strFilePath));
	{
		DeleteFile(strFilePath);
	}*/
	// 清空文件
	//DeleteDirectories(strDestPath); 
	/*if (FALSE == ::CreateDirectory(strDestPath, NULL))
	{
		AfxMessageBox("创建解压路径失败");
		return;
	}*/
	if (!PathIsDirectory(strDestPath))
	{

		::CreateDirectory(strDestPath, NULL);

	}
	//x解压  -ibck后台执行 -o+如存在，则覆盖 -inul不弹出错误提示  
	//使用 -ibck，缩小到了系统托盘区域
	CString strCmd = "\"" + winRarInstallPath + "\" x -ibck -o+ -inul \"" + strFilePath + "\" \"" + strDestPath + "\"";
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	BOOL bRet = CreateProcess(NULL, strCmd.GetBuffer(MAX_PATH), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	DWORD dwExit = 0;
	if (bRet)
	{
		//这个地方将导致该函数为阻塞状态  
		WaitForSingleObject(pi.hProcess, INFINITE);
		::GetExitCodeProcess(pi.hProcess, &dwExit);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	HWND OrderHwnd = ::FindWindow("ADCT", "工单选择");
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
		LPBYTE getValue1 = new BYTE[80];//得到的键值
		DWORD keyType = REG_SZ;//定义数据类型
		DWORD DataLen = 80;//定义数据长度
		CString strUser = _T("exe32");//要查询的键名称
		long ret1 = ::RegQueryValueEx(hKEY, strUser, NULL, &keyType, getValue1, &DataLen);

		if (ret1 == ERROR_SUCCESS)
		{
			winRarInstallPath = (char*)getValue1;
		}
		else
		{
			AfxMessageBox("错误：无法查询到WINRAR的注册表信息");
		}
	}
	else if (ret64 == ERROR_SUCCESS)
	{
		hKEY = hKEY64;
		LPBYTE getValue2 = new BYTE[80];//得到的键值
		DWORD keyType = REG_SZ;//定义数据类型
		DWORD DataLen = 80;//定义数据长度
		CString strUser64 = _T("exe64");//要查询的键名称
		long ret2 = ::RegQueryValueEx(hKEY, strUser64, NULL, &keyType, getValue2, &DataLen);
		if (ret2 == ERROR_SUCCESS)
		{
			winRarInstallPath = (char*)getValue2;
		}
		else
		{
			AfxMessageBox("错误：无法查询到WINRAR的注册表信息");
		}
	}
	else//如果无法打开hKEY,则中止程序的执行
	{
		AfxMessageBox("错误：无法打开有关的hKEY");
		return;
	}
	
	
	
	
}