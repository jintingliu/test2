
// BIBToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "vector"
#include "map"
#include <io.h>
#include <fcntl.h> 
#include <TableHead.h>
using namespace std;
// CBIBToolDlg 对话框
class CBIBToolDlg : public CDialogEx
{
	// 构造
public:
	CBIBToolDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_BIBTOOL_DIALOG };

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
	char bListTitle[BLOCK_TEST_ITEMS_NUM_MAX][20];
	BYTE bUnitType[BLOCK_TEST_ITEMS_NUM_MAX];
	//CString *strListBuff[8 * 1024][BLOCK_TEST_ITEMS_NUM_MAX + 1];
	//CString *strListBuff;
	CStringArray strListBuff[BLOCK_TEST_ITEMS_NUM_MAX + 1];
	CString strMsgSelect[40];
	CString list_colTitle[32];

	
	

	afx_msg void AddBBTHeadMsg(CString str, BYTE bMsgIdx);
	afx_msg void DecodeBBTHead(BYTE *bReadBuff);
	afx_msg void ToolInit(void);
	afx_msg void LoadBBTFile(CString strFilePath);
	afx_msg void SetDataMode(BYTE bMode);
	afx_msg void OnBnClickedButton1();
	CListCtrl list_allItem;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CButton btn_outputXls;
	CButton btn_openFile;
	CEdit edt_BBTHeadMsg;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CButton cb_hexMode;
	afx_msg void OnBnClickedCbHexmode();
	CComboBox cbo_msgSelect;
	afx_msg void OnCbnSelchangeComboMsgSelect();
	CButton btn_clearList;
	afx_msg void OnBnClickedBtnClearList();
};
