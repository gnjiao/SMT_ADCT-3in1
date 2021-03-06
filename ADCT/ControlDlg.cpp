// ControlDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ADCT.h"
#include "ControlDlg.h"
#include "afxdialogex.h"
#include "OrderDlg.h"
#include "PublicFunction.h"


// ControlDlg 对话框

IMPLEMENT_DYNAMIC(ControlDlg, CDialogEx)

ControlDlg::ControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Control_Dialog, pParent)
{

}

ControlDlg::~ControlDlg()
{
}

void ControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TestCotrol_Btn, &ControlDlg::OnBnClickedTestcotrolBtn)
	ON_BN_CLICKED(IDC_CFGControl_Btn, &ControlDlg::OnBnClickedCfgcontrolBtn)
END_MESSAGE_MAP()


// ControlDlg 消息处理程序


void ControlDlg::OnBnClickedTestcotrolBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	OrderDlg *Dlg = new OrderDlg();
	this->EndDialog(0);
	CDialogEx::OnOK();
	Dlg->DoModal();
}


void ControlDlg::OnBnClickedCfgcontrolBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	OpenADCT_CFG();
}

void ControlDlg::OpenADCT_CFG()
{
	CPublicFunction *CPF = new CPublicFunction();
	string temp = CPF->GetCurDir() + "ADCT_CFG\\ADCT_CFG.exe";
	ShellExecute(NULL, "open", temp.c_str(), NULL, NULL, SW_SHOW);
	exit(0);
}