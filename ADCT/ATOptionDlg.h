#pragma once


// ATOptionDlg 对话框

class ATOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ATOptionDlg)

public:
	ATOptionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ATOptionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AutoTestOption_Dialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ATPortType;
	bool m_ATReTest;
	bool m_ATTD;  //自动测试 泰斗
	CComboBox ATPortType_CK;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
