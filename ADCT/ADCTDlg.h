
// ADCTDlg.h: 头文件
//

#pragma once



extern HWND MultiDownloadHWND;
extern HWND ATEHWND[4];
extern HWND AutoTestHWND;
extern HWND DLOptionHWND;
extern CString SleepTimer;
extern float ATELossList[8][10];
extern bool DeBugMode;
extern HWND MainFormHwnd;
#define CleanInitLog 1001
#define CheckStopStatus 1002
#define OpenPower 1003
#define PowerSendMessage 10001
#define PowerReadMessage 10002


// CADCTDlg 对话框
class CADCTDlg : public CDialogEx
{
	// 构造
public:
	CADCTDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADCT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSimplemessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainformmessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMastermanagermessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	int COMPortType;
	CString m_BaudCCB;
	CString m_DownloadTypeCCB;
	int DownloadFormatOperation;
	int FlashType;
	CComboBox AllPortCCB[4];
	afx_msg void OnClose();
	LRESULT OnSetTextMessage(WPARAM wParam, LPARAM lParam);
	void OnMessageSend(int SendFor, int MessageNum, WPARAM WParam, LPARAM LParam);
	afx_msg
		void ADCTCopyDataSend(int SendFor, char  Port[1024], int Status, int MessageNum, char  MessageChar[1024]);
	void ATESharedMemory(char Port[1024], int Status, int MessageNum, char MessageChar[1024]);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	int CheckHWND(HWND hwnd);
	void MultiDownloadCopyDataManage(CString &PortStr, COPYDATASTRUCT * pCopyDataStruct, ADCTMessage * m_ADCTMessage);
	void AutoTestCopyDataManage(COPYDATASTRUCT * pCopyDataStruct);
	void ATECopyDataManage(CWnd* pWnd, COPYDATASTRUCT * pCopyDataStruct);
	void MultiDownloadInit();
	void AutoTestInit();
	void ATEInit();
	void LockOption();
	void UnlockOption();
	void GetMainIniConfig(void);
	void SetMainIniConfig();
	void GetMultiDownloadIniConfig();
	afx_msg void OnBnClickedDloptionBtn();
	void SetMultiDownloadOption();
	void CheckToolStatus();
	void SendOptionForMultiDownload();
	void SimplePortBtn(int Operate, int PortIndex, char *Port);
	afx_msg void OnSimplePortStartClick(UINT nID);
	void OnSimplePortStopClick(UINT nID);
	afx_msg void OnBnClickedPortallstartBtn();
	afx_msg void OnBnClickedPortallstopBtn();
	void InitProgressControl();
	void MultiDownloadManager();
	CString OrderNameStatic;
	CString VersionStatic;
	CString MultiDownloadFilePath;
	CString NVRAMFilePath;
	CString PowerCom;
	void PowerSet(int Address, bool PowerAction);
	void CommandListSet(int At, CString str);
	void OpenMultiDownload();
	void OpenAutoTest();
	void OpenATE();
	void ShowLog(int IndexControl, CString szLog, int TextColor);
	CRichEditCtrl m_ctrlRichEditLog[4];
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_TipsLB[4];
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString CutStringc(CString str);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	bool ThreadManager(bool active);
	bool CheckStartStatus();
	bool ListenStartBtn(int BtnControl);
	int COMPortToIndex(CString COMPort);
	CString ATPortType;
	bool ATReTest;
	bool ATTD;  //自动测试 泰斗


	afx_msg void OnBnClickedAtoptionBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	void SetStopStatus(int PortIndex);
	void CheckPortUser(int PortIndex);
	HWND ATEPortToHWND(CString Port);
	afx_msg void OnBnClickedAtesetBtn();
	void SetATEGPIBAddress();
	CStringArray m_GPIBList;
	afx_msg void OnBnClickedAdctsetBtn();
	CString m_DownLoadRetry;
	CString m_ATERetry;
	CString m_AutoTestRetry;
};
