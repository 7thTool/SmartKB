
// mfcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc.h"
#include "mfcDlg.h"
#include "afxdialogex.h"
#include <vector>

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


// CmfcDlg �Ի���



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


// CmfcDlg ��Ϣ�������


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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	for (size_t i = 0; i < 10000; i++)
	{
		SMARTKBITEM item = { 0 };
		_stprintf(item.szKey1, _T("%d"), i*i / (i+1) % 323);
		_stprintf(item.szKey2, _T("%d"), i*i / (i + 1) % 999);
		_stprintf(item.szName, _T("%d"), i);
		g_items.push_back(item);
	}
	g_items.push_back({ _T("����"), _T("����"), _T("����") });
	g_items.push_back({ _T("����1"), _T("����4"), _T("����sdad") });
	g_items.push_back({ _T("����2"), _T("����5"), _T("����asd") });
	g_items.push_back({ _T("����3"), _T("����6"), _T("����ads") });

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SMartKB_Init(&my_callback);
	SMartKB_AddItem(&g_items[0], g_items.size());
	mSmartKB = SMartKB_Create(GetSafeHwnd(), SW_SHOW);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmfcDlg::OnPaint()
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
HCURSOR CmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmfcDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString strText;
	mEdit1.GetWindowText(strText);
	SMartKB_Input(strText);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CmfcDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	SMartKB_Term();
	mSmartKB = NULL;
}


BOOL CmfcDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
