
// MouseMovementDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MouseMovement.h"
#include "MouseMovementDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMouseMovementDlg dialog



CMouseMovementDlg::CMouseMovementDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUSEMOVEMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	movX = 0;
}

void CMouseMovementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITMOVEMENT, m_EditMovement);
	DDX_Control(pDX, IDC_STATICSTATUS, m_editStatus);
}

BEGIN_MESSAGE_MAP(CMouseMovementDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_INPUT()
	ON_BN_CLICKED(IDOK, &CMouseMovementDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMouseMovementDlg message handlers

BOOL CMouseMovementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMouseMovementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMouseMovementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CMouseMovementDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	RAWINPUTDEVICE Rid[2];

	Rid[1].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[1].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	Rid[1].dwFlags = RIDEV_INPUTSINK;    // adds mouse and also ignores legacy mouse messages
	Rid[1].hwndTarget = m_hWnd;

	Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x06;              // HID_USAGE_GENERIC_KEYBOARD
	Rid[0].dwFlags = RIDEV_INPUTSINK;    // adds keyboard and also ignores legacy keyboard messages
	Rid[0].hwndTarget = m_hWnd;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{

		OutputDebugStringW(L"Registration Error!\n");
	}
	measuring = false;

	return 0;
}


void CMouseMovementDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your message handler code here and/or call default

	UINT dwSize = 0;

	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	if (GetRawInputData(hRawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{

		if (raw->data.keyboard.VKey == VK_OEM_PERIOD && raw->data.keyboard.Flags == RI_KEY_BREAK) {

			if (measuring) {
				measuring = false;
				OutputDebugStringW(L"Stopped Measuring.\n");
				m_editStatus.SetWindowTextW(L"Waiting...");
				CString s;
				s.Format(_T("%d"), movX);
				m_EditMovement.SetWindowTextW(s);
			}
			else {
				measuring = true;
				movX = 0;
				m_editStatus.SetWindowTextW(L"Measuring...");
				OutputDebugStringW(L"Started Measuring.\n");
			}
		}

	}
	else if (raw->header.dwType == RIM_TYPEMOUSE && measuring)
	{
		movX += raw->data.mouse.lLastX;
	}

	delete[] lpb;

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}




void CMouseMovementDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
