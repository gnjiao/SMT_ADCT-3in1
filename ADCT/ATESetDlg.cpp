// ATESetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ADCT.h"
#include "ATESetDlg.h"
#include "afxdialogex.h"
#include "ADCTDlg.h"


// ATESetDlg 对话框

IMPLEMENT_DYNAMIC(ATESetDlg, CDialogEx)

ATESetDlg::ATESetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ATESet_Dialog, pParent)
{

}

ATESetDlg::~ATESetDlg()
{
}

void ATESetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATELossList_ListControl, m_ATELossList_ListCtrl);
	DDX_Control(pDX, IDC_List_TB, m_edit);
}


BEGIN_MESSAGE_MAP(ATESetDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_ATELossList_ListControl, &ATESetDlg::OnNMDblclkAtelosslistListcontrol)
	ON_EN_KILLFOCUS(IDC_List_TB, &ATESetDlg::OnEnKillfocusListTb)
	ON_BN_CLICKED(IDOK, &ATESetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ATESetDlg 消息处理程序


BOOL ATESetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_ATELossList_ListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_ATELossList_ListCtrl.SetExtendedStyle(dwStyle);
	m_ATELossList_ListCtrl.SetRowHeigt(20);
	m_ATELossList_ListCtrl.InsertColumn(0, "GPIB地址");
	m_ATELossList_ListCtrl.InsertColumn(1, "串口号");
	m_ATELossList_ListCtrl.InsertColumn(2, "GSM850 OUT LOSS");
	m_ATELossList_ListCtrl.InsertColumn(3, "GSM850 INP LOSS");
	m_ATELossList_ListCtrl.InsertColumn(4, "GSM900 OUT LOSS");
	m_ATELossList_ListCtrl.InsertColumn(5, "GSM900 INP LOSS");
	m_ATELossList_ListCtrl.InsertColumn(6, "DCS1800 OUT LOSS");
	m_ATELossList_ListCtrl.InsertColumn(7, "DCS1800 INP LOSS");
	m_ATELossList_ListCtrl.InsertColumn(8, "PCS1900 OUT LOSS");
	m_ATELossList_ListCtrl.InsertColumn(9, "PCS1900 INP LOSS");
	for (int i = IDC_GPIBAddress1_TB; i < IDC_GPIBAddress1_TB+4; i++)
	{
		GetDlgItem(i)->SetWindowText(GPIBList.GetAt(i - IDC_GPIBAddress1_TB));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void ATESetDlg::ChangeCtrl(CMyListCtrl* ControlName)
{
	CRect rc;
	if (m_Col == 0)
	{
		return;
	}
	ControlName->GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获取RECT；
	m_edit.SetParent(ControlName);//转换坐标为列表框中的坐标
	m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
	m_edit.SetWindowText(ControlName->GetItemText(m_Row, m_Col));//将值放在Edit；
	m_edit.ShowWindow(SW_SHOW);//显示Edit控件；
	m_edit.SetFocus();//设置Edit焦点
	m_edit.ShowCaret();//显示光标
	m_edit.SetSel(-1);//将光标移动到最后
}
void ATESetDlg::GetLoss()
{
	CString FloatToStr;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			FloatToStr.Format("%f", ATELossList[i][j]);
			if (j==0)
			{
				m_ATELossList_ListCtrl.InsertItem(i, FloatToStr);
			}
			else
			{
				m_ATELossList_ListCtrl.SetItemText(i, j, FloatToStr);
			}
			
		}
		
	}
	
}

void ATESetDlg::OnNMDblclkAtelosslistListcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_Row = pNMListView->iItem;//获得选中的行
	m_Col = pNMListView->iSubItem;//获得选中列
	ChangeCtrl(&m_ATELossList_ListCtrl);
	*pResult = 0;
}


void ATESetDlg::OnEnKillfocusListTb()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tem;
	m_edit.GetWindowText(tem);
	m_ATELossList_ListCtrl.SetItemText(m_Row, m_Col, tem);   //设置编辑框的新内容
	m_edit.ShowWindow(SW_HIDE);
}


void ATESetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString TextTemp;
	GPIBList.RemoveAll();
	GPIBList.SetSize(4);
	for (int i = IDC_GPIBAddress1_TB; i < IDC_GPIBAddress1_TB+4; i++)
	{
		GetDlgItem(i)->GetWindowText(TextTemp);
		GPIBList.SetAt(i - IDC_GPIBAddress1_TB,TextTemp);
	}
	
	CDialogEx::OnOK();
}
