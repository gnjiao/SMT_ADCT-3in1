#pragma once


// ADCTSetDlg 对话框

class ADCTSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ADCTSetDlg)

public:
	ADCTSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ADCTSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADCTSet_Dialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString DownLoadRetry;
	CString ATERetry;
	CString AutoTestRetry;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
