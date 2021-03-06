// ATOptionDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ADCT.h"
#include "ATOptionDlg.h"
#include "afxdialogex.h"


// ATOptionDlg 对话框

IMPLEMENT_DYNAMIC(ATOptionDlg, CDialogEx)

ATOptionDlg::ATOptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AutoTestOption_Dialog, pParent)
{

}

ATOptionDlg::~ATOptionDlg()
{
}

void ATOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_ATPortType, ATPortType_CK);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ATOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ATOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ATOptionDlg 消息处理程序


BOOL ATOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ATPortType_CK.AddString("终端---稳定端口");
	ATPortType_CK.AddString("终端---跳动端口");
	ATPortType_CK.SetWindowText("终端---稳定端口");
	((CButton *)GetDlgItem(IDC_ATReTest_CK))->SetCheck(m_ATReTest);
	((CButton *)GetDlgItem(IDC_ATTD_CK))->SetCheck(m_ATTD);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ATOptionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ATPortType_CK.GetWindowText(m_ATPortType);
	m_ATReTest =((CButton *)GetDlgItem(IDC_ATReTest_CK))->GetCheck();
	m_ATTD=((CButton *)GetDlgItem(IDC_ATTD_CK))->GetCheck();
	CDialogEx::OnOK();
}
