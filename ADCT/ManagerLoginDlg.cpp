// ManagerLoginDlg.cpp: 实现文件
//

#include "stdafx.h"
#include <string>
#include "ADCT.h"
#include "ManagerLoginDlg.h"
#include "afxdialogex.h"
#include "SQLManager.h"


// ManagerLoginDlg 对话框

IMPLEMENT_DYNAMIC(ManagerLoginDlg, CDialogEx)

ManagerLoginDlg::ManagerLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OptionLogin_Dlg, pParent)
{

}

ManagerLoginDlg::~ManagerLoginDlg()
{
}

void ManagerLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ManagerLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ManagerLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ManagerLoginDlg 消息处理程序


void ManagerLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ManagerName, ManagerPwd;
	GetDlgItemText(IDC_ManagerName_TB, ManagerName);
	GetDlgItemText(IDC_ManagerPwd_TB, ManagerPwd);
	_RecordsetPtr getRecord;
	SQLManager sqlManager;
	CString CheckStr = "";
	CString CheckType = "";
	if (!SQLIsUsing)
	{
		SQLIsUsing = true;
		if (sqlManager.ConnSQL())
		{
			try
			{
				getRecord = sqlManager.SQLCommand("SELECT *"
					" FROM [GPSTest].[dbo].[LUserAccount]"
					" WHERE Name = '" + ManagerName + "' AND Password='" + ManagerPwd + "'"
				);
				while (!getRecord->adoEOF) {
					CheckStr = getRecord->GetCollect("Name");
					CheckType = getRecord->GetCollect("UserType");
					getRecord->MoveNext();
				}
			}
			catch (const std::exception&)
			{

			}
			sqlManager.CloseAll();
			SQLIsUsing = false;
		}
		else
		{
			AfxMessageBox("数据库连接失败");
		}
	}
	if (CheckStr!="")
	{
		if (CheckType.Find("&00")==-1)
		{
			AfxMessageBox("该账户没有管理员权限");
			return;
		}
		else
		{
			CDialogEx::OnOK();
		}
	}
	else
	{
		AfxMessageBox("用户名或密码错误");
	}
	
	/*if (ManagerName=="1")
	{
		if (ManagerPwd == "1")
		{
			CDialogEx::OnOK();
		}
	}
	else
	{
		return;
	}*/
	
}


BOOL ManagerLoginDlg::PreTranslateMessage(MSG* pMsg)
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
