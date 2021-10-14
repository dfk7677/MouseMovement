
// MouseMovementDlg.h : header file
//

#pragma once


// CMouseMovementDlg dialog
class CMouseMovementDlg : public CDialogEx
{
// Construction
public:
	CMouseMovementDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSEMOVEMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
private:
	bool measuring;
	long movX;
public:
	afx_msg void OnStnClickedStaticmovement();
private:
	CEdit m_EditMovement;
public:
	afx_msg void OnBnClickedOk();
	CStatic m_editStatus;
};
