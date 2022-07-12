
// LoadGenerateDlg.h: 头文件
//

#pragma once
#include "CLoaderShellCode.h"

// CLoadGenerateDlg 对话框
class CLoadGenerateDlg : public CDialogEx
{
// 构造
public:
	CLoadGenerateDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADGENERATE_DIALOG };
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_edtFlieBin;
	BOOL m_checkMAC;
	BOOL m_checREG;

	afx_msg void OnBnClickedButton2();
	BOOL m_checkRMM;
	BOOL m_checkUUID;
};
