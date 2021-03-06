#pragma once
#include "CMyListCtrl.h"

// ATESetDlg 对话框

class ATESetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ATESetDlg)

public:
	ATESetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ATESetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ATESet_Dialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ChangeCtrl(CMyListCtrl * ControlName);
	void GetLoss();
	CMyListCtrl m_ATELossList_ListCtrl;
	int m_Row;
	int m_Col;
	CEdit m_edit;
	afx_msg void OnNMDblclkAtelosslistListcontrol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusListTb();
	afx_msg void OnBnClickedOk();
	CStringArray GPIBList;
};
