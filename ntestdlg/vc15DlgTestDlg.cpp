
// ntestdlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "vc15DlgTest.h"
#include "vc15DlgTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cvc15testdlgDlg �Ի���



Cvc15testdlgDlg::Cvc15testdlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VC15TESTDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cvc15testdlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, TestDlgListBox, m_listBox);
	DDX_Control(pDX, TestDlgCheckCombboxEx, m_ckcb);
}

BEGIN_MESSAGE_MAP(Cvc15testdlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(WM_TEST_ICON_MSG, OnRClickIconMsg)
	ON_BN_CLICKED(MY_OPEN, ResumeDlg)
	ON_BN_CLICKED(MY_HIDE, HideDlg)
	ON_BN_CLICKED(MY_QUITE, OnExitApp)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(TestDlgCheckCombboxEx, &Cvc15testdlgDlg::OnCbnSelchangeTestdlgcheckcombboxex)
END_MESSAGE_MAP()


// Cvc15testdlgDlg ��Ϣ��������

BOOL Cvc15testdlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	CrtTrayIcon();

	m_ckcb.AddString(_T("���"));
	m_ckcb.AddString(_T("����"));
	m_ckcb.AddString(_T("�ò���"));
	m_ckcb.AddString(_T("�ð�"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cvc15testdlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cvc15testdlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cvc15testdlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL Cvc15testdlgDlg::CrtTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	// 	nid.hWnd = this->m_hWnd;
	nid.hWnd = GetSafeHwnd();
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	nid.uCallbackMessage = WM_TEST_ICON_MSG;//�Զ������Ϣ���� 
	lstrcpy(nid.szTip, _T("���ԶԻ���"));
	BOOL bRes = Shell_NotifyIcon(NIM_ADD, &nid);
	return bRes;
}

BOOL Cvc15testdlgDlg::DelTrayIcon()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	// 	nid.hWnd = this->m_hWnd;
	nid.hWnd = GetSafeHwnd();
	nid.uID = IDR_MAINFRAME;
// 	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
// 	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
// 	nid.uCallbackMessage = WM_TEST_ICON_MSG;//�Զ������Ϣ���� 
// 	lstrcpy(nid.szTip, _T("���ԶԻ���"));
	BOOL bRes = Shell_NotifyIcon(NIM_DELETE, &nid);
	return bRes;
}

void Cvc15testdlgDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
	if (nType == SIZE_MINIMIZED)
	{
		//��С�����ڣ�������
		HideDlg();
	}
}

void Cvc15testdlgDlg::HideDlg()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = GetSafeHwnd();
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	nid.uCallbackMessage = WM_TEST_ICON_MSG;//�Զ������Ϣ���� 
	lstrcpy(nid.szTip, _T("������..."));
	BOOL bRes = Shell_NotifyIcon(NIM_MODIFY, &nid);
	ShowWindow(SW_HIDE);
}

void Cvc15testdlgDlg::ResumeDlg()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
// 	nid.hWnd = this->m_hWnd;
	nid.hWnd = GetSafeHwnd();
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	nid.uCallbackMessage = WM_TEST_ICON_MSG;//�Զ������Ϣ���� 
	lstrcpy(nid.szTip, _T("���ԶԻ���"));
	BOOL bRes = Shell_NotifyIcon(NIM_MODIFY, &nid);
	ShowWindow(SW_NORMAL);
}

LRESULT Cvc15testdlgDlg::OnRClickIconMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ�� 
		CMenu menu;
		UINT nState;

		menu.CreatePopupMenu();//����һ������ʽ�˵� 
							   //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ��� 
							   //���أ�������������� 
		menu.AppendMenu(MF_STRING, MY_OPEN, _T("��"));
		nState = IsWindowVisible() ? MF_GRAYED : MF_ENABLED;
		nState = MF_BYCOMMAND | nState;
		menu.EnableMenuItem(MY_OPEN, nState);

		menu.AppendMenu(MF_STRING, MY_HIDE, _T("����"));
		nState = IsWindowVisible() ? MF_ENABLED: MF_GRAYED;
		nState = MF_BYCOMMAND | nState;
		menu.EnableMenuItem(MY_HIDE, nState);

		menu.AppendMenu(MF_STRING, MY_QUITE, _T("�ر�"));
		//ȷ������ʽ�˵���λ�� 
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//��Դ���� 
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		ResumeDlg();
	}
	default:
		break;
	}
	return 0;
}

void Cvc15testdlgDlg::OnExitApp()
{
	OnOK();
}


void Cvc15testdlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	DelTrayIcon();
}


void Cvc15testdlgDlg::OnCbnSelchangeTestdlgcheckcombboxex()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
}