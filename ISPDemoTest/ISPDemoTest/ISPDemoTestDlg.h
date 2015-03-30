
// ISPDemoTestDlg.h : header file
//

#pragma once


// CISPDemoTestDlg dialog
class CISPDemoTestDlg : public CDialogEx
{
// Construction
public:
	CISPDemoTestDlg(CWnd* pParent = NULL);	// standard constructor

public:
	BYTE *m_pImageRaw;

// Dialog Data
	enum { IDD = IDD_ISPDEMOTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFileOpenRaw();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedConvertBayer10bit();
	afx_msg void OnBnClickedButton1();
};
