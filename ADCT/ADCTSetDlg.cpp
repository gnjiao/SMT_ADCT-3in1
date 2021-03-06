// ADCTSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ADCT.h"
#include "ADCTSetDlg.h"
#include "afxdialogex.h"


// ADCTSetDlg 对话框

IMPLEMENT_DYNAMIC(ADCTSetDlg, CDialogEx)

ADCTSetDlg::ADCTSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADCTSet_Dialog, pParent)
{

}

ADCTSetDlg::~ADCTSetDlg()
{
}

void ADCTSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ADCTSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ADCTSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ADCTSetDlg 消息处理程序


BOOL ADCTSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_DownloadRetry_TB)->SetWindowText(DownLoadRetry);
	GetDlgItem(IDC_ATERetry_TB)->SetWindowText(ATERetry);
	GetDlgItem(IDC_AutoTestRetry_TB)->SetWindowText(AutoTestRetry);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





void ADCTSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_DownloadRetry_TB)->GetWindowText(DownLoadRetry);
	GetDlgItem(IDC_ATERetry_TB)->GetWindowText(ATERetry);
	GetDlgItem(IDC_AutoTestRetry_TB)->GetWindowText(AutoTestRetry);
	CDialogEx::OnOK();
}
