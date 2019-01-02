
// mfcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc.h"
#include "mfcDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CmfcDlg 对话框



CmfcDlg::CmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mEdit1);
}

BEGIN_MESSAGE_MAP(CmfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CmfcDlg::OnEnChangeEdit1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CmfcDlg 消息处理程序


/*SMARTKBITEM g_items[] = {
	{ _T("1"), _T("1"), _T("1"), 1 }
	,{ _T("22"), _T("22"), _T("22"), 2 }
	,{ _T("333"), _T("333"), _T("333"), 3 }
};*/
std::vector<SMARTKBITEM> g_items;

void my_callback(PSMARTKBITEM pItem)
{
	if (pItem)
	{
		CString str;
		str.Format(_T("%s %s %s\n"), pItem->szKey1, pItem->szKey2, pItem->szName);
		AfxMessageBox(str);
	}
}


BOOL CmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	for (size_t i = 0; i < 10000; i++)
	{
		SMARTKBITEM item = { 0 };
		_stprintf(item.szKey1, _T("%d"), i*i / (i+1) % 323);
		_stprintf(item.szKey2, _T("%d"), i*i / (i + 1) % 999);
		_stprintf(item.szName, _T("%d"), i);
		g_items.push_back(item);
	}
	g_items.push_back({ _T("中文"), _T("中文"), _T("中文") });
	g_items.push_back({ _T("中文1"), _T("中文4"), _T("中文sdad") });
	g_items.push_back({ _T("中文2"), _T("中文5"), _T("中文asd") });
	g_items.push_back({ _T("中文3"), _T("中文6"), _T("中文ads") });

	// TODO: 在此添加额外的初始化代码
	SMartKB_Init(&my_callback);
	SMartKB_AddItem(&g_items[0], g_items.size());
	mSmartKB = SMartKB_Create(GetSafeHwnd(), SW_SHOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmfcDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString strText;
	mEdit1.GetWindowText(strText);
	SMartKB_Input(strText);

	// TODO:  在此添加控件通知处理程序代码
}


void CmfcDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	SMartKB_Term();
	mSmartKB = NULL;
}


BOOL CmfcDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (&mEdit1 == GetFocus())
		{
			switch (pMsg->wParam)
			{
			case VK_UP:
			case VK_DOWN:
			case VK_RETURN:
				return SMartKB_Control(pMsg->wParam);
				break;
			default:
				break;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
