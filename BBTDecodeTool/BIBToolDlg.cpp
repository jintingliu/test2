
// BIBToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BIBTool.h"
#include "BIBToolDlg.h"
#include "afxdialogex.h"
#include "TableHead.h"

WORD itemIndexMap[8 * 1024];

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DB_PRINT printf

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBIBToolDlg 对话框



CBIBToolDlg::CBIBToolDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CBIBToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBIBToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LOADFILE, btn_openFile);
	DDX_Control(pDX, IDC_LIST2, list_allItem);
	DDX_Control(pDX, IDC_BTN_OUTPUTFILE, btn_outputXls);
	DDX_Control(pDX, IDC_EDIT2, edt_BBTHeadMsg);
	DDX_Control(pDX, IDC_CB_HEXMODE, cb_hexMode);
	DDX_Control(pDX, IDC_COMBO_MSG_SELECT, cbo_msgSelect);
	DDX_Control(pDX, IDC_BTN_CLEAR_LIST, btn_clearList);
}

BEGIN_MESSAGE_MAP(CBIBToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOADFILE, &CBIBToolDlg::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_OUTPUTFILE, &CBIBToolDlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CBIBToolDlg::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_CB_HEXMODE, &CBIBToolDlg::OnBnClickedCbHexmode)
	ON_CBN_SELCHANGE(IDC_COMBO_MSG_SELECT, &CBIBToolDlg::OnCbnSelchangeComboMsgSelect)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LIST, &CBIBToolDlg::OnBnClickedBtnClearList)
END_MESSAGE_MAP()


// CBIBToolDlg 消息处理程序

BOOL CBIBToolDlg::OnInitDialog()
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

	// TODO:  在此添加额外的初始化代码
	ToolInit();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CBIBToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBIBToolDlg::OnPaint()
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
HCURSOR CBIBToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBIBToolDlg::ToolInit(void)
{
	strcpy_s(bListTitle[TEST_ITEM_BBT_SLC], "SLC ECC");
	strcpy_s(bListTitle[TEST_ITEM_BBT_TLC], "TLC ECC");
	strcpy_s(bListTitle[TEST_ITEM_AP_ERASE_SLC], "SLC 擦电流");
	strcpy_s(bListTitle[TEST_ITEM_AP_ERASE_TLC], "TLC 擦电流");
	strcpy_s(bListTitle[TEST_ITEM_AP_PROGRAM_SLC], "SLC 写电流");
	strcpy_s(bListTitle[TEST_ITEM_AP_PROGRAM_TLC], "TLC 写电流");
	strcpy_s(bListTitle[TEST_ITEM_AP_READ_SLC], "SLC 读电流");
	strcpy_s(bListTitle[TEST_ITEM_AP_READ_TLC], "TLC 读电流");
	strcpy_s(bListTitle[TEST_ITEM_tERASE_SLC], "SLC 擦时间");
	strcpy_s(bListTitle[TEST_ITEM_tERASE_TLC], "TLC 擦时间");
	strcpy_s(bListTitle[TEST_ITEM_tPROGRAM_SLC], "SLC 写时间");
	strcpy_s(bListTitle[TEST_ITEM_tPROGRAM_TLC], "TLC 写时间");
	strcpy_s(bListTitle[TEST_ITEM_tREAD_SLC], "SLC 读时间");
	strcpy_s(bListTitle[TEST_ITEM_tREAD_TLC], "TLC 读时间");
	strcpy_s(bListTitle[TEST_ITEM_BLOCK_PE_CYCLE], "PECycle");

	memset((BYTE *)bUnitType, TEST_ITEM_NULL, sizeof(bUnitType));

	for (int j = 0; j <= 8*1024; j++)
	{
		itemIndexMap[j] = j;
	}
}

void CBIBToolDlg::AddBBTHeadMsg(CString str, BYTE bMsgIdx)
{
	//int iLen = edt_BBTHeadMsg.GetWindowTextLength();
	//edt_BBTHeadMsg.SetSel(iLen, iLen, TRUE);
	//edt_BBTHeadMsg.ReplaceSel(str, FALSE);
	////edt_BBTHeadMsg.SetWindowTextW(str);

	strMsgSelect[bMsgIdx] += str;
}

DWORD CalculateDataXorValue(DWORD *pdwDataBuff, DWORD dwByteLen, DWORD dwOrigXorValue)
{
	DWORD i;
	DWORD dwXorResult;

	dwXorResult = dwOrigXorValue;

	for (i = 0; i < (dwByteLen / sizeof(DWORD)); i++)
	{
		dwXorResult ^= pdwDataBuff[i];
	}

	return dwXorResult;
}

BYTE CheckBBT(BYTE *bReadBuff)
{
	BYTE bRetVal = TRUE;

	DB_PRINT("\r\n[CheckBBT]");

	//校验BBT Flag
	if (OFFLINE_FLAG != ((tyOfflineScanTableHead *)bReadBuff)->dwStructFlag)
	{
		DB_PRINT("\r\n[CheckBBT] BBT结构体标志校验失败!");
		//MessageBox(NULL, _T("BBT结构体标志校验失败!"), _T("提示"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//校验BBT Head尺寸
	if (STRUCT_SIZE_TABLE_HEAD != ((tyOfflineScanTableHead *)bReadBuff)->dwStructSize)
	{
		DB_PRINT("\r\n[CheckBBT] BBT结构体尺寸校验失败!");
		//MessageBox(NULL, _T("BBT结构体尺寸校验失败!"), _T("提示"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//校验BBT 结构体版本
	if (OFFLINE_VER != ((tyOfflineScanTableHead *)bReadBuff)->wStructVersion)
	{
		DB_PRINT("\r\n[CheckBBT] BBT结构体版本校验失败!");
		//MessageBox(NULL, _T("BBT结构体版本校验失败!"), _T("提示"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//校验BBT 结构体异或校验值
	if (CalculateDataXorValue((DWORD *)bReadBuff, (1024 - 4), 0) != ((tyOfflineScanTableHead *)bReadBuff)->dwChecksum)
	{
		DB_PRINT("\r\n[CheckBBT] BBT结构体异或校验值校验失败!");
		//MessageBox(NULL, _T("BBT结构体异或校验值校验失败!"), _T("提示"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

CheckBBT_Out:

	return bRetVal;
}

void CBIBToolDlg::DecodeBBTHead(BYTE *bReadBuff)
{
	char bWriteBuff[1024];

	DB_PRINT("\r\n[DecodeBBTHead]");

	BYTE bMsgIdx = (BYTE)((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionNum;
	CString str;
	str.Format(_T("%d"), bMsgIdx);
	str = _T("BBT分段: ") + str;
	if (cbo_msgSelect.FindString(0, str) < 0)
	{
		cbo_msgSelect.AddString(str);
	}
	
	sprintf_s(bWriteBuff, "BBT结构体版本: 0x%04X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->wStructVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT总page数: 0x%04X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->wResultPageTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "EMMC版本: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwEmmcSortingVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32CB版本: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32CBVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32TU版本: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32TUVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32RU版本: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32RUVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "Port号: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwBIBPortNumber);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "是否完成扫描: %s \r\n", (((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bTestCompletion) ? "是" : "否");
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "工作电压: %s \r\n", (((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingVoltage) ? ((((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingVoltage) == 1 ? "低压" : "高压") : "标压");
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "工作温度: %d \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingTemperature);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "SLC阶段VCCQ最大值: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSlcBurninVccqMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "TLC阶段VCCQ最大值: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wTlcBurninVccqMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "低电压异常记录次数: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wVDTCnt);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试时间: %u 毫秒\r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.dwDuration);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT分段号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "上一个BBT分段所在Page号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wLastSectionPos);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT分段总数: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "当前BBT分段起始Block号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionStartBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "当前分段测试Block数: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionBlockTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试项数目: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bTestItemNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "1个测试结果的长度: %u 字节\r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bBUByteLen);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试完成项标记位: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.dwTestItemDoneBit);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "私有块号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[0].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "备份块号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[1].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "Debug块号: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[2].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试起始温度: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwStart);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试结束温度: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwEnd);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试过程最高温度: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "测试过程最低温度: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwMin);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);

	//如果表格是空的才生成列标题
	if (list_allItem.GetItemCount() <= 0)
	{
		BYTE bTestItemNum = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bTestItemNum;

		for (BYTE i = 0; i < bTestItemNum; i++)
		{
			bUnitType[i] = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.tTestItemInfo[i].Type;
		}

		//生成列标题
		list_allItem.InsertColumn(0, _T("block号"));
		list_allItem.SetColumnWidth(0, 75);

		/*CString strListTitle[15] = { _T("SLC ECC"), _T("TLC ECC"), _T("SLC 擦电流"), _T("TLC 擦电流"), _T("SLC 写电流"),
			_T("TLC 写电流"), _T("SLC 读电流"), _T("TLC 读电流"), _T("SLC 擦时间"), _T("TLC 擦时间"),
			_T("SLC 写时间"), _T("TLC 写时间"), _T("SLC 读时间"), _T("TLC 读时间"), _T("PECycle") };*/

		for (BYTE i = 0; i < bTestItemNum; i++)
		{
			list_allItem.InsertColumn(i + 1, CString(bListTitle[bUnitType[i]], strlen(bListTitle[bUnitType[i]])));
			list_allItem.SetColumnWidth(i + 1, 100);
		}
		list_allItem.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_ALIGNLEFT | LVS_AUTOARRANGE | LVS_EX_FULLROWSELECT);
	}
}

void CBIBToolDlg::LoadBBTFile(CString strFilePath)
{
	BYTE bReadBuff[2048] = { 0 };
	char bfilePath[200] = { 0 };
	char bWriteBuff[1024];
	CFile file;

	DB_PRINT("\n%S", strFilePath.GetBuffer(0));
	file.Open(strFilePath, CFile::modeRead | CFile::typeBinary);

	//如果文件长度小于8K，肯定不是BBT文件
	if (file.GetLength() < (8 * 1024))
	{
		DB_PRINT("\r\n[ReadBBTFile] 文件长度小于8K!");
		MessageBox(strFilePath + _T("文件长度小于8K!"), _T("提示"), MB_OK | MB_ICONWARNING);

		file.Close();
		return;
	}

	//检查BBT头（前1K数据）
	int itemByteLen;
	int testItemNum;
	int readSize = file.Read(bReadBuff, 1024);

	if (FALSE == CheckBBT(bReadBuff))
	{
		file.Close();
		return;
	}
	DecodeBBTHead(bReadBuff);

	itemByteLen = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bBUByteLen;
	testItemNum = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bTestItemNum;

	int itemNumStart = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionStartBlockNum;
	int itemNum = itemNumStart;
	int itemNumEnd = itemNum + ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionBlockTotal;

	//如果当前读取的BBT记录文件顺序不是最开始的位置，将之前的项目填充NULL数据
	if (itemNumStart >= list_allItem.GetItemCount())
	{
		for (int i = list_allItem.GetItemCount(); i < itemNumStart; i++)
		{
			list_allItem.InsertItem(i, _T("nihao"));
			CString str;
			str.Format(_T("0x%04X"), i);
			list_allItem.SetItemText(i, 0, str);
			//载入缓存
			strListBuff[0].InsertAt(i, str);

			for (int j = 0; j < testItemNum; j++)
			{
				list_allItem.SetItemText(i, j + 1, _T("NULL"));
				//载入缓存
				strListBuff[j + 1].InsertAt(i, _T("NULL"));
			}
		}
	}

	//获取每一个测试单元的字节长度
	BYTE bUnitByteLen[BLOCK_TEST_ITEMS_NUM_MAX] = { 0 };
	for (int i = 0; i < testItemNum; i++)
	{
		bUnitByteLen[i] = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.tTestItemInfo[i].ByteLen;
	}

	//逐项读取item
	while (readSize = file.Read(bReadBuff, itemByteLen))
	{
		if (readSize != itemByteLen)
		{
			DB_PRINT("\r\n[ReadBBTFile] 文件读取出现错误!");
			MessageBox(_T("文件读取出现错误!"), _T("提示"), MB_OK | MB_ICONWARNING);
			break;
		}

		//创建新行，并插入对应的block号
		if (itemNum >= list_allItem.GetItemCount())
		{
			list_allItem.InsertItem(itemNum, _T("nihao"));
			CString str;
			str.Format(_T("0x%04X"), itemNum);
			list_allItem.SetItemText(itemNum, 0, str);
			//载入缓存
			strListBuff[0].InsertAt(itemNum, str);
		}
		

		BYTE bByteOffset = 0;
		for (int i = 0; i < testItemNum; i++)
		{
			if (1 == bUnitByteLen[i])
			{
				sprintf_s(bWriteBuff, "0x%02X", *(BYTE *)(bReadBuff + bByteOffset));
			}
			else
			{
				sprintf_s(bWriteBuff, "0x%04X", *(WORD *)(bReadBuff + bByteOffset));
			}
			bByteOffset += bUnitByteLen[i];

			//将解析的Unit数据插入到表格中
			list_allItem.SetItemText(itemNum, i + 1, CString(bWriteBuff));
			//载入缓存
			if (itemNum >= strListBuff[i + 1].GetCount())
			{
				strListBuff[i + 1].InsertAt(itemNum, CString(bWriteBuff));
			}
			else
			{
				strListBuff[i + 1].SetAt(itemNum, CString(bWriteBuff));
			}
		}

		itemNum++;

		if (itemNum >= itemNumEnd)
		{
			break;
		}
	}

	//将排序结果展示到列表控件上
	for (int j = 0; j < list_allItem.GetItemCount(); j++)
	{
		itemIndexMap[j] = j;
	}
	SetDataMode(cb_hexMode.GetCheck());

	file.Close();
}

void CBIBToolDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*|");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter, this);
	CString strFilePath;

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//最多允许同时打开20个文件
	fileDlg.m_ofn.lpstrFile = pBuffer;
	fileDlg.m_ofn.nMaxFile = MAX_PATH * 20;
	fileDlg.m_ofn.lpstrFile[0] = '\0';

	if (fileDlg.DoModal() == IDOK)
	{
		POSITION  pos = fileDlg.GetStartPosition();

		while (pos != NULL)
		{
			//根据文件路径以二进制读取方式打开文件
			strFilePath = fileDlg.GetNextPathName(pos);
			LoadBBTFile(strFilePath);
		}
	}
}


void CBIBToolDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UINT count;
	TCHAR filePath[MAX_PATH] = { 0 };

	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//从成功的拖放操作中检索文件的名称。并取代被拖拽文件的数目
	
	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));//获得拖拽的文件名
		LoadBBTFile(filePath);
	}
	UpdateData(FALSE);
	DragFinish(hDropInfo);//拖放成功后，释放内存

	CDialogEx::OnDropFiles(hDropInfo);
}

void CBIBToolDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	DB_PRINT("\r\n[Output cvs File]");
	CFile fileWrite;
	char bWriteBuff[1024];

	TCHAR szFilter[] = _T("数据表文件(*.csv)|*.csv|文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("csv"), _T("my"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// 显示保存文件对话框   
	if (IDOK == fileDlg.DoModal())
	{ 
		strFilePath = fileDlg.GetPathName();

		if (strFilePath.GetLength() <= 0)
		{
			DB_PRINT("\r\n[SaveFile]没有选择文件夹!");
			return;
		}

		

		//创建对应的文件
		fileWrite.Open(strFilePath, CFile::modeWrite | CFile::modeRead | CFile::modeCreate);

		//写入BBT头信息
		int BBTTotalNum = cbo_msgSelect.GetCount();
		for (int i = 0; i < BBTTotalNum; i++)
		{
			
			CString str;
			char dBuf[4096];
			cbo_msgSelect.GetWindowTextW(str);
			str = strMsgSelect[wcstol(str.Mid(str.GetLength() - 1), NULL, 10)];
			WideCharToMultiByte(CP_OEMCP, 0, str.GetBuffer(), -1, NULL, 0, NULL, FALSE);
			WideCharToMultiByte(CP_OEMCP, 0, str.GetBuffer(), -1, dBuf, 0, NULL, FALSE);
			fileWrite.Write(dBuf, strlen(dBuf));
			fileWrite.Write(",", 1);
			/*int n = str.Find(_T(" "));
			fileWrite.Write(bWriteBuff, strlen(bWriteBuff));*/
		}
		fileWrite.Write("\n", 1);

		sprintf_s(bWriteBuff, "block号,");
		fileWrite.Write(bWriteBuff, strlen(bWriteBuff));

		//写入列标题
		for (int col = 0; bUnitType[col] != TEST_ITEM_NULL; col++)
		{
			sprintf_s(bWriteBuff, "%s,", bListTitle[bUnitType[col]]);
			fileWrite.Write(bWriteBuff, strlen(bWriteBuff));

		}
		fileWrite.Write("\n", 1);

		//逐行写入item
		int itemTotalNum = list_allItem.GetItemCount();
		for (int line = 0; line < itemTotalNum; line++)
		{
			for (int col = 0; col < 16; col++)
			{
				sprintf_s(bWriteBuff, "%S,", list_allItem.GetItemText(line, col));
				fileWrite.Write(bWriteBuff, strlen(bWriteBuff));
			}
			fileWrite.Write("\n", 1);
		}
		fileWrite.Close();
	}
}


void CBIBToolDlg::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int colSelect = pNMListView->iSubItem;//获得当前所选列

	static BYTE bSortType = 1;  //0，升序排列；1，降序排列

	//获取总行数
	int lineTotal = list_allItem.GetItemCount();

	//strListBuff里的每一行数据都是顺序排列的，做排序算法时不去交换buff里的数据，而是将排序结果记在一个映射数组里
	for (int j = 0; j <= lineTotal; j++)
	{
		itemIndexMap[j] = j;
	}

	//冒泡排序
	for (int j = 0; j < lineTotal - 1; j++)
	{
		for (int i = 0; i<lineTotal - 1 - j; i++)
		{

			CString temp1 = strListBuff[colSelect].GetAt(itemIndexMap[i]);
			CString temp2 = strListBuff[colSelect].GetAt(itemIndexMap[i + 1]);

			if ((temp1 < temp2) && (bSortType == 1))
			{
				int tempInt = itemIndexMap[i];
				itemIndexMap[i] = itemIndexMap[i + 1];
				itemIndexMap[i + 1] = tempInt;
			}
			else if ((temp1 > temp2) && (bSortType == 0))
			{
				int tempInt = itemIndexMap[i];
				itemIndexMap[i] = itemIndexMap[i + 1];
				itemIndexMap[i + 1] = tempInt;
			}

		}
	}

	SetDataMode(cb_hexMode.GetCheck());

	bSortType = bSortType == 1 ? 0 : 1;
	*pResult = 0;
}

void CBIBToolDlg::SetDataMode(BYTE bMode)
{
	if (0 == bMode)  //十进制
	{
		//将排序结果展示到列表控件上
		for (int line = 0; line < list_allItem.GetItemCount(); line++)
		{
			for (int col = 0; col < 16; col++)
			{
				char buf[100];
				CString str = strListBuff[col].GetAt(itemIndexMap[line]);
				if (str == _T("NULL"))
				{
					list_allItem.SetItemText(line, col, str);
				}
				else
				{
					sprintf_s(buf, "%d", wcstol(str, NULL, 16));
					list_allItem.SetItemText(line, col, CString(buf));
				}
			}
		}
	}
	else  //十六进制
	{
		//将排序结果展示到列表控件上
		for (int line = 0; line < list_allItem.GetItemCount(); line++)
		{
			for (int col = 0; col < 16; col++)
			{
				char buf[100];
				list_allItem.SetItemText(line, col, strListBuff[col].GetAt(itemIndexMap[line]));
			}
		}
	}
}

void CBIBToolDlg::OnBnClickedCbHexmode()
{
	// TODO:  在此添加控件通知处理程序代码
	SetDataMode(cb_hexMode.GetCheck());
}


void CBIBToolDlg::OnCbnSelchangeComboMsgSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	cbo_msgSelect.GetWindowTextW(str);
	
	edt_BBTHeadMsg.SetWindowTextW(strMsgSelect[wcstol(str.Mid(str.GetLength() - 1), NULL, 10)]);
	//edt_BBTHeadMsg.SetWindowTextW(str);
}


void CBIBToolDlg::OnBnClickedBtnClearList()
{
	// TODO:  在此添加控件通知处理程序代码
	memset((BYTE *)bUnitType, TEST_ITEM_NULL, sizeof(bUnitType));

	for (int j = 0; j <= 8 * 1024; j++)
	{
		itemIndexMap[j] = j;
	}

	int colTotal = list_allItem.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < colTotal; i++)
	{
		list_allItem.DeleteColumn(0);
	}

	list_allItem.DeleteAllItems();
	cbo_msgSelect.ResetContent();
	edt_BBTHeadMsg.SetWindowTextW(_T(""));
}
