// OrderDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ADCT.h"
#include "afxdialogex.h"
#include "OrderDlg.h"
#include "SQLManager.h"
#include "ADCTDlg.h"
#include <string>
#include "FTPClass.h"
#include "PublicFunction.h"
#include "WinRARClass.h"

CString Version;
CString FTPSoftwarePath, IniPath, IniLocalPath, CFGPath, SetupPath;
int SoftwareSize, IniSize, CFGSize, SetupSize;
CStringArray AllOrderList;
CStringArray FuzzySearchOrderList;
CString SCFileFolderPath;
CStringArray FindFileArray;
CString m_MultiDownloadFilePath;
CString m_NVRAMFilePath;
CString m_OrderName, m_VersionName;
bool DownloadSuccessList[4] = { false,false,false,false };

CStringArray CFGFileList, SetupFileList;
// OrderDlg 对话框


using namespace std;
IMPLEMENT_DYNAMIC(OrderDlg, CDialogEx)

OrderDlg::OrderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Order_Dialog, pParent)
{

}

OrderDlg::~OrderDlg()
{
}

void OrderDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_OrderName_CCB, m_OrderName_CCB);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OrderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ReadOption_Btn, &OrderDlg::OnBnClickedReadoptionBtn)
	ON_CBN_EDITCHANGE(IDC_OrderName_CCB, &OrderDlg::OnCbnEditchangeOrdernameCcb)
	ON_MESSAGE(WM_OrderDlgMessage, &OrderDlg::OnOrderdlgmessage)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SimpleMessage, &OrderDlg::OnSimplemessage)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

BOOL OrderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Download_PG = ((CProgressCtrl *)GetDlgItem(IDC_OrderDownload_PG));

	m_Download_PG->SetStep(10);
	m_Download_PG->SetPos(0);
	WinRARClass winrar;
	winrar.FindWinRARPath();
	SelectAllOrder();
	::SendMessage(ADCTBackEndHWND, WM_SimpleMessage, 1, (LPARAM)AfxGetMainWnd()->m_hWnd);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// OrderDlg 消息处理程序

void OrderDlg::SelectAllOrder()
{
	_RecordsetPtr getRecord;
	SQLManager sqlManager;
	if (!SQLIsUsing)
	{
		SQLIsUsing = true;
		if (sqlManager.ConnSQL())
		{
			try
			{
				AllOrderList.RemoveAll();
				getRecord = sqlManager.SQLCommand("SELECT ZhiDan FROM [GPSTest].[dbo].[Gps_ManuOrderParam] where Status=0 OR Status=1");
				int OrderNum = 0;
				while (!getRecord->adoEOF) {
					AllOrderList.Add(getRecord->GetCollect("ZhiDan"));
					OrderNum++;
					getRecord->MoveNext();
					//AfxMessageBox(ReadID);
				}
			}
			catch (const std::exception&)
			{

			}
			sqlManager.CloseAll();

		}
		SQLIsUsing = false;


	}
}

bool OrderDlg::Download(CString IPAddress, CString Port, CString UserName, CString Password, CString fileName, CString serverFileName, int serverFileSize)
{
	FTPClass download;
	CPublicFunction *CPF = new CPublicFunction();
	string temp = CPF->GetCurDir();
	temp += "src\\";
	if (download.FTPConnect(IPAddress, atoi(Port), UserName, Password))
	{
		fileName.Replace("/", "\\");
		CString CutTemp = fileName.Right(fileName.GetLength() - 1);
		CString LocalPathTemp = temp.c_str() + CutTemp;
		m_downloadInfo.LocalPath = LocalPathTemp;
		CPF->CreateDirectoryRecursionFun(LocalPathTemp, 0);
		m_downloadInfo.ServerFileSize = serverFileSize;
		if (download.FindFile(serverFileName))
		{
			//SetTimer(DownloadStatus_Timer, 100, NULL);

			download.DownloadStart();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		AfxMessageBox("FTP连接失败");
		return false;
	}

}

void OrderDlg::OnBnClickedReadoptionBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 4; i++)
	{
		DownloadSuccessList[i] = false;
	}
	GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(false);
	GetDlgItem(IDC_OrderName_CCB)->GetWindowText(m_OrderName);
	GetDlgItem(IDC_Version_TB)->GetWindowText(m_VersionName);
	_RecordsetPtr getRecord;
	SQLManager sqlManager;
	Version = "";
	CString AutoTestOrderCheck;
	if (!SQLIsUsing)
	{
		SQLIsUsing = true;
		if (sqlManager.ConnSQL())
		{
			try
			{
				getRecord = sqlManager.SQLCommand("SELECT  [Gps_ManuOrderParam].ZhiDan,[Gps_ManuOrderParam].Version, "
					"[Gps_ManuOrderParam].Status, [ADCT_Version].*,ADCT_Config.* FROM [GPSTest].[dbo].[Gps_ManuOrderParam], "
					"[GPSTest].[dbo].[ADCT_Version],[GPSTest].[dbo].[ADCT_Config] WHERE Gps_ManuOrderParam.Version = ADCT_Version.VersionName AND "
					"Gps_ManuOrderParam.ZhiDan = '" + m_OrderName + "' AND Gps_ManuOrderParam.Version='" + m_VersionName + "' AND Gps_ManuOrderParam.Version = ADCT_Config.Version "
					"and (Gps_ManuOrderParam.Status=0 or Gps_ManuOrderParam.Status=1)"
				);
				while (!getRecord->adoEOF) {
					Version = getRecord->GetCollect("VersionName");
					FTPSoftwarePath = getRecord->GetCollect("Path");
					SoftwareSize = getRecord->GetCollect("Size");
					IniPath = getRecord->GetCollect("IniPath");
					CFGPath = getRecord->GetCollect("CFGPath");
					SetupPath = getRecord->GetCollect("SetupPath");
					IniSize = getRecord->GetCollect("IniSize");
					CFGSize = getRecord->GetCollect("CFGSize");
					SetupSize = getRecord->GetCollect("SetupSize");
					getRecord->MoveNext();
					//AfxMessageBox(ReadID);

				}

				getRecord = sqlManager.SQLCommand("SELECT * FROM [NetMarkIMEI].[dbo].[TestSystemSettingFunc]"
					" WHERE"
					" SoftWare = '" + Version + m_OrderName + "'"
				);
				while (!getRecord->adoEOF) {
					AutoTestOrderCheck = getRecord->GetCollect("SoftWare");
					getRecord->MoveNext();
				}

			}
			catch (const std::exception&)
			{
				AfxMessageBox("订单查询时，数据库连接失败");
			}
			sqlManager.CloseAll();

		}
		SQLIsUsing = false;
		if (Version == "")
		{
			AfxMessageBox("无工单号或软件版本匹配,请查询订单状态和软件包状态");
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
			return;
		}
		if (AutoTestOrderCheck == "")
		{
			AfxMessageBox("自动测试订单单号或软件版本不匹配");
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
			return;
		}
		if (Download(sqlSettingInfo.FTPAddress, sqlSettingInfo.FTPPort, sqlSettingInfo.FTPUserName, sqlSettingInfo.FTPPWD, FTPSoftwarePath, FTPSoftwarePath, SoftwareSize))
		{
			SendMessage(WM_OrderDlgMessage, MessageDownloadStart, SoftwareSize);
		}
		else
		{
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
		}


	}

}





void OrderDlg::OnCbnEditchangeOrdernameCcb()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_KeyWords;
	m_OrderName_CCB.GetWindowText(m_KeyWords);
	m_OrderName_CCB.ResetContent();
	FuzzySearch(AllOrderList, m_KeyWords, FuzzySearchOrderList);
	int OrderNum = FuzzySearchOrderList.GetSize();
	CRect rc;
	m_OrderName_CCB.GetDroppedControlRect(&rc);
	m_OrderName_CCB.GetParent()->ScreenToClient(&rc);
	rc.bottom += 10;
	m_OrderName_CCB.MoveWindow(&rc);
	for (int i = 0; i < OrderNum; i++)
	{
		m_OrderName_CCB.AddString(FuzzySearchOrderList.GetAt(i));
	}


	m_OrderName_CCB.ShowDropDown(true);
	m_OrderName_CCB.SetWindowText(m_KeyWords);
	m_OrderName_CCB.SetEditSel(m_KeyWords.GetLength() + 1, m_KeyWords.GetLength() + 1);
	SendMessage(WM_SETCURSOR, 0, 0);

	//ReleaseCapture();
}

void OrderDlg::FuzzySearch(CStringArray &FuzzySearchInList, CString KeyWords, CStringArray &FuzzySearchOutList)
{
	FuzzySearchOutList.RemoveAll();
	int MaxNum = FuzzySearchInList.GetSize();
	CString NowSearch;
	for (int i = 0; i < MaxNum; i++)
	{
		NowSearch = FuzzySearchInList.GetAt(i);
		if (NowSearch.Find(KeyWords) > -1)
		{
			FuzzySearchOutList.Add(NowSearch);
		}
	}
}

void OrderDlg::FindAllFiles(const char * lpPath)
{

	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy_s(szFind, lpPath);
	strcat_s(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
			{
				//发现子目录，递归
				char szFile[MAX_PATH] = { 0 };
				strcpy_s(szFile, lpPath);
				strcat_s(szFile, "\\");
				strcat_s(szFile, FindFileData.cFileName);
				FindAllFiles(szFile);
			}
		}
		else
		{
			//找到文件
			char szFile[MAX_PATH] = { 0 };
			strcpy_s(szFile, lpPath);
			strcat_s(szFile, "\\");
			strcat_s(szFile, FindFileData.cFileName);
			FindFileArray.Add(szFile);
		}
	} while (::FindNextFile(hFind, &FindFileData));

	::FindClose(hFind);

}

afx_msg LRESULT OrderDlg::OnOrderdlgmessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case MessageDownloadStart:
	{
		m_Download_PG->SetRange32(0, lParam);
		SetTimer(DownloadSizeCheck_Timer, 100, NULL);
	}
	break;
	case MessageDownloadSuccess:
	{
		for (int i = 0; i < 4; i++)
		{
			if (!DownloadSuccessList[i])
			{
				DownloadSuccessList[i] = true;
				if (i == 0)
				{

					CPublicFunction pf;
					CString sCurPath = pf.GetCurDir();
					WinRARClass winrar;
					FTPSoftwarePath.Replace("/", "\\");
					SCFileFolderPath = sCurPath + "src\\" + FTPSoftwarePath.Right(FTPSoftwarePath.GetLength() - 1);
					winrar.UnpackFile(SCFileFolderPath);
				}
				else if (i == 1)
				{
					SendMessage(WM_OrderDlgMessage, MessageDownloadIniSuccess, 0);
				}
				else if (i == 2)
				{
					SendMessage(WM_OrderDlgMessage, MessageDownloadCFGSuccess, 0);
				}
				else if (i == 3)
				{
					SendMessage(WM_OrderDlgMessage, MessageDownloadSetupSuccess, 0);
				}
				break;
			}
		}


	}
	break;
	case MessageUnpackSuccess:
	{
		//AfxMessageBox("解压完成");
		CString Folder = SCFileFolderPath.Left(SCFileFolderPath.ReverseFind('.'));
		FindFileArray.RemoveAll();
		FindAllFiles(Folder);
		m_Download_PG->StepIt();
		SendMessage(WM_OrderDlgMessage, MessageFindFileSuccess, 0);
	}
	break;
	case MessageFindFileSuccess:
	{
		DisposeDownloadFile();
		m_Download_PG->StepIt();
		SendMessage(WM_OrderDlgMessage, MessageDownloadSoftwareSuccess, 0);
		//AfxMessageBox("遍历完成");

		//MainForm.ShowWindow(true);

	}
	break;
	case MessageDownloadSoftwareSuccess:
	{
		if (Download(sqlSettingInfo.FTPAddress, sqlSettingInfo.FTPPort, sqlSettingInfo.FTPUserName, sqlSettingInfo.FTPPWD, IniPath, IniPath, IniSize))
		{
			SendMessage(WM_OrderDlgMessage, MessageDownloadStart, IniSize);

		}
		else
		{
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
		}


	}
	break;
	case MessageDownloadIniSuccess:
	{
		if (Download(sqlSettingInfo.FTPAddress, sqlSettingInfo.FTPPort, sqlSettingInfo.FTPUserName, sqlSettingInfo.FTPPWD, CFGPath, CFGPath, CFGSize))
		{
			SendMessage(WM_OrderDlgMessage, MessageDownloadStart, CFGSize);
		}
		else
		{
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
		}
	}
	break;
	case MessageDownloadCFGSuccess:
	{
		if (Download(sqlSettingInfo.FTPAddress, sqlSettingInfo.FTPPort, sqlSettingInfo.FTPUserName, sqlSettingInfo.FTPPWD, SetupPath, SetupPath, SetupSize))
		{
			SendMessage(WM_OrderDlgMessage, MessageDownloadStart, SetupSize);
		}
		else
		{
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
		}
	}
	break;
	case MessageDownloadSetupSuccess:
	{
		CPublicFunction *CPF = new CPublicFunction();
		string temp = CPF->GetCurDir();
		temp += "src\\";
		CFGPath.Replace("/", "\\");
		SetupPath.Replace("/", "\\");
		CString CFGLocalPath = temp.c_str() + CFGPath.Right(CFGPath.GetLength() - 1);
		CString SetupLocalPath = temp.c_str() + SetupPath.Right(SetupPath.GetLength() - 1);
		IniPath.Replace("/", "\\");
		IniLocalPath = temp.c_str() + IniPath.Right(IniPath.GetLength() - 1);
		CString LocalCFGName = CFGLocalPath.Left(CFGLocalPath.ReverseFind('.'));
		CString LocalSetupName = SetupLocalPath.Left(SetupLocalPath.ReverseFind('.'));
		CString CFGType = CFGLocalPath.Right(CFGLocalPath.GetLength() - CFGLocalPath.ReverseFind('.'));
		CString SetupType = SetupLocalPath.Right(SetupLocalPath.GetLength() - SetupLocalPath.ReverseFind('.'));
		CString FormatTemp;
		CFGFileList.SetSize(4);
		SetupFileList.SetSize(4);
		CString InitCFGPath, InitSetup;
		for (int i = 0; i < 4; i++)
		{
			FormatTemp.Format("_%d", i);
			InitCFGPath = LocalCFGName + FormatTemp;
			InitCFGPath += CFGType;
			CFGFileList.SetAt(i, InitCFGPath);
			InitSetup = LocalSetupName + FormatTemp;
			InitSetup += SetupType;
			SetupFileList.SetAt(i, InitSetup);
			HANDLE Handle;
			while (1)
			{
				Handle = CreateFile(CFGLocalPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE != Handle)
				{
					break;
				}

			}
			CloseHandle(Handle);
			if (!CopyFile(CFGLocalPath, CFGFileList.GetAt(i), FALSE))
			{
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf,
					0,
					NULL
				);
				MessageBox((LPCTSTR)lpMsgBuf);
				LocalFree(lpMsgBuf);
				AfxMessageBox("创建CFG文件副本失败");
				GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
				return 0;
			}
			while (1)
			{
				Handle = CreateFile(SetupLocalPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE != Handle)
				{
					break;
				}

			}
			CloseHandle(Handle);
			if (!CopyFile(SetupLocalPath, SetupFileList.GetAt(i), FALSE))
			{
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf,
					0,
					NULL
				);
				MessageBox((LPCTSTR)lpMsgBuf);
				LocalFree(lpMsgBuf);
				AfxMessageBox("创建Setup文件副本失败");
				GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
				return 0;
			}

			//Sleep(1000);
		}

		if (!DeleteFile(CFGLocalPath))
		{
			AfxMessageBox("删除原CFG文件失败");
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
			return 0;
		}
		if (!DeleteFile(SetupLocalPath))
		{
			AfxMessageBox("删除原Setup文件失败");
			GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
			return 0;
		}
		SendMessage(WM_OrderDlgMessage, MessageCopyFileSuccess, 0);
	}
	break;
	case MessageCopyFileSuccess:
	{
		CString PathTemp;
		for (int i = 0; i < 4; i++)
		{
			PathTemp = m_NVRAMFilePath;
			PathTemp.Replace("\\", "\\\\");
			WritePrivateProfileString("System Setting", "FDM database file", PathTemp.GetBuffer(), SetupFileList.GetAt(i));
			PathTemp = IniLocalPath;
			PathTemp.Replace("\\", "\\\\");
			WritePrivateProfileString("System Setting", "Calibration file", PathTemp.GetBuffer(), SetupFileList.GetAt(i));
			PathTemp = CFGFileList.GetAt(i);
			PathTemp.Replace("\\", "\\\\");
			WritePrivateProfileString("System Setting", "Config file", PathTemp.GetBuffer(), SetupFileList.GetAt(i));
		}
		SendMessage(WM_OrderDlgMessage, MessageSetupFileSuccess, 0);
	}
	break;
	case MessageSetupFileSuccess:
	{
		GetDlgItem(IDC_ReadOption_Btn)->EnableWindow(true);
		CADCTDlg MainForm;
		this->EndDialog(0);
		MainForm.OrderNameStatic = m_OrderName;
		MainForm.VersionStatic = m_VersionName;
		MainForm.NVRAMFilePath = m_NVRAMFilePath;
		MainForm.MultiDownloadFilePath = m_MultiDownloadFilePath;
		MainForm.DoModal();
	}
	break;
	default:
		break;
	}
	return 0;
}

void OrderDlg::DisposeDownloadFile()
{
	int FileCountCheck = 0;
	for (int i = 0; i < FindFileArray.GetSize(); i++)
	{
		if (FindFileArray.GetAt(i).Find(".cfg") > 0)
		{
			FileCountCheck++;
			m_MultiDownloadFilePath = FindFileArray.GetAt(i);
			m_MultiDownloadFilePath = m_MultiDownloadFilePath.Left(m_MultiDownloadFilePath.ReverseFind('\\'));
			//MainForm.ADCTCopyDataSend(1, "ALL", 1, 3, m_MultiDownloadFilePath.GetBuffer());
			//AfxMessageBox(MultiDownloadFilePath);
		}
		if (FindFileArray.GetAt(i).Find("BPL") > -1)
		{
			FileCountCheck++;
			m_NVRAMFilePath = FindFileArray.GetAt(i);
		}
	}
	if (FileCountCheck != 2)
	{
		AfxMessageBox("文件缺失");
		return;
	}
}

void OrderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case DownloadSizeCheck_Timer:
	{
		m_Download_PG->SetPos(m_downloadInfo.LocalFileSize);
		if (m_downloadInfo.LocalFileSize == m_downloadInfo.ServerFileSize)
		{
			KillTimer(DownloadSizeCheck_Timer);
		}
	}
	break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL OrderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE: //Esc event
			return true;
		case VK_RETURN: //Enter event 
			return true;
		case VK_SPACE:
			return true;
		default:
			;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



afx_msg LRESULT OrderDlg::OnSimplemessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


BOOL OrderDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pWnd->m_hWnd== ADCTBackEndHWND)
	{
		ADCTMessage *m_ADCTMessage = new ADCTMessage;
		m_ADCTMessage = (ADCTMessage*)pCopyDataStruct->lpData;
		CString temp, PortStr, MessageCharStr;
		PortStr = m_ADCTMessage->Port;
		MessageCharStr = m_ADCTMessage->MessageChar;
		switch (pCopyDataStruct->dwData)
		{
		case 0:
		{

		}
		break;
		default:
			break;
		}
	}
	
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
