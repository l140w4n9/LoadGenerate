﻿
// LoadGenerateDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LoadGenerate.h"
#include "LoadGenerateDlg.h"
#include "afxdialogex.h"

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


// CLoadGenerateDlg 对话框



CLoadGenerateDlg::CLoadGenerateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADGENERATE_DIALOG, pParent)
	, m_edtFlieBin(_T(""))
	, m_checkMAC(FALSE)
	, m_checREG(FALSE)
	, m_checkRMM(FALSE)
	, m_checkUUID(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
}

void CLoadGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edtFlieBin);
	DDX_Check(pDX, IDC_CHECK1, m_checkMAC);
	DDX_Check(pDX, IDC_CHECK4, m_checREG);
	DDX_Check(pDX, IDC_CHECK3, m_checkRMM);
	DDX_Check(pDX, IDC_CHECK2, m_checkUUID);
}

BEGIN_MESSAGE_MAP(CLoadGenerateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BUT_BIN, &CLoadGenerateDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoadGenerateDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLoadGenerateDlg 消息处理程序

BOOL CLoadGenerateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoadGenerateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoadGenerateDlg::OnPaint()
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
HCURSOR CLoadGenerateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoadGenerateDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlgFile(TRUE);

	CString fileBin;

	if(dlgFile.DoModal() == IDOK)
	{
		fileBin = dlgFile.GetPathName();
	}

	m_edtFlieBin = fileBin;
	UpdateData(FALSE);

	
}


void CLoadGenerateDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CLoaderShellCode shellcode;

	CStringA stra((TCHAR*)(LPCTSTR)(m_edtFlieBin));
	string strFileBin((char*)(LPCSTR)(stra));
	if(!shellcode.FileToLoadCode(strFileBin.data()))
	{
		AfxMessageBox("打开Bin文件失败");
		return;
	}

	UpdateData(TRUE);

	if(m_checREG)
	{
		shellcode.CreateREG();
	}
	if (m_checkMAC)
	{
		shellcode.CreateMAC();
	}
	if (m_checkUUID)
	{
		shellcode.CreateUUID();
	}
	if (m_checkRMM)
	{
		shellcode.CreateRMM();
	}
}
