#pragma once


// ControlDlg 对话框

class ControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ControlDlg)

public:
	ControlDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ControlDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Control_Dialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTestcotrolBtn();
	afx_msg void OnBnClickedCfgcontrolBtn();
	void OpenADCT_CFG();
};
