
// BIBToolDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CBIBToolDlg �Ի���



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


// CBIBToolDlg ��Ϣ�������

BOOL CBIBToolDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	ToolInit();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBIBToolDlg::OnPaint()
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
HCURSOR CBIBToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBIBToolDlg::ToolInit(void)
{
	strcpy_s(bListTitle[TEST_ITEM_BBT_SLC], "SLC ECC");
	strcpy_s(bListTitle[TEST_ITEM_BBT_TLC], "TLC ECC");
	strcpy_s(bListTitle[TEST_ITEM_AP_ERASE_SLC], "SLC ������");
	strcpy_s(bListTitle[TEST_ITEM_AP_ERASE_TLC], "TLC ������");
	strcpy_s(bListTitle[TEST_ITEM_AP_PROGRAM_SLC], "SLC д����");
	strcpy_s(bListTitle[TEST_ITEM_AP_PROGRAM_TLC], "TLC д����");
	strcpy_s(bListTitle[TEST_ITEM_AP_READ_SLC], "SLC ������");
	strcpy_s(bListTitle[TEST_ITEM_AP_READ_TLC], "TLC ������");
	strcpy_s(bListTitle[TEST_ITEM_tERASE_SLC], "SLC ��ʱ��");
	strcpy_s(bListTitle[TEST_ITEM_tERASE_TLC], "TLC ��ʱ��");
	strcpy_s(bListTitle[TEST_ITEM_tPROGRAM_SLC], "SLC дʱ��");
	strcpy_s(bListTitle[TEST_ITEM_tPROGRAM_TLC], "TLC дʱ��");
	strcpy_s(bListTitle[TEST_ITEM_tREAD_SLC], "SLC ��ʱ��");
	strcpy_s(bListTitle[TEST_ITEM_tREAD_TLC], "TLC ��ʱ��");
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

	//У��BBT Flag
	if (OFFLINE_FLAG != ((tyOfflineScanTableHead *)bReadBuff)->dwStructFlag)
	{
		DB_PRINT("\r\n[CheckBBT] BBT�ṹ���־У��ʧ��!");
		//MessageBox(NULL, _T("BBT�ṹ���־У��ʧ��!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//У��BBT Head�ߴ�
	if (STRUCT_SIZE_TABLE_HEAD != ((tyOfflineScanTableHead *)bReadBuff)->dwStructSize)
	{
		DB_PRINT("\r\n[CheckBBT] BBT�ṹ��ߴ�У��ʧ��!");
		//MessageBox(NULL, _T("BBT�ṹ��ߴ�У��ʧ��!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//У��BBT �ṹ��汾
	if (OFFLINE_VER != ((tyOfflineScanTableHead *)bReadBuff)->wStructVersion)
	{
		DB_PRINT("\r\n[CheckBBT] BBT�ṹ��汾У��ʧ��!");
		//MessageBox(NULL, _T("BBT�ṹ��汾У��ʧ��!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);

		bRetVal = FALSE;
		goto CheckBBT_Out;
	}

	//У��BBT �ṹ�����У��ֵ
	if (CalculateDataXorValue((DWORD *)bReadBuff, (1024 - 4), 0) != ((tyOfflineScanTableHead *)bReadBuff)->dwChecksum)
	{
		DB_PRINT("\r\n[CheckBBT] BBT�ṹ�����У��ֵУ��ʧ��!");
		//MessageBox(NULL, _T("BBT�ṹ�����У��ֵУ��ʧ��!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);

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
	str = _T("BBT�ֶ�: ") + str;
	if (cbo_msgSelect.FindString(0, str) < 0)
	{
		cbo_msgSelect.AddString(str);
	}
	
	sprintf_s(bWriteBuff, "BBT�ṹ��汾: 0x%04X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->wStructVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT��page��: 0x%04X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->wResultPageTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "EMMC�汾: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwEmmcSortingVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32CB�汾: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32CBVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32TU�汾: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32TUVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "STM32RU�汾: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwSTM32RUVersion);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "Port��: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->dwBIBPortNumber);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "�Ƿ����ɨ��: %s \r\n", (((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bTestCompletion) ? "��" : "��");
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "������ѹ: %s \r\n", (((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingVoltage) ? ((((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingVoltage) == 1 ? "��ѹ" : "��ѹ") : "��ѹ");
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "�����¶�: %d \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.bWorkingTemperature);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "SLC�׶�VCCQ���ֵ: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSlcBurninVccqMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "TLC�׶�VCCQ���ֵ: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wTlcBurninVccqMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "�͵�ѹ�쳣��¼����: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wVDTCnt);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "����ʱ��: %u ����\r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.dwDuration);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT�ֶκ�: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "��һ��BBT�ֶ�����Page��: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wLastSectionPos);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "BBT�ֶ�����: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "��ǰBBT�ֶ���ʼBlock��: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionStartBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "��ǰ�ֶβ���Block��: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.wSectionBlockTotal);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "��������Ŀ: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bTestItemNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "1�����Խ���ĳ���: %u �ֽ�\r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bBUByteLen);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "�����������λ: 0x%08X \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.dwTestItemDoneBit);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "˽�п��: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[0].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "���ݿ��: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[1].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "Debug���: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tScanPriBlk[2].wBlockNum);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "������ʼ�¶�: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwStart);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "���Խ����¶�: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwEnd);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "���Թ�������¶�: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwMax);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);
	sprintf_s(bWriteBuff, "���Թ�������¶�: %u \r\n", ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tTemperatureValue.dwMin);
	AddBBTHeadMsg(CString(bWriteBuff), bMsgIdx);

	//�������ǿյĲ������б���
	if (list_allItem.GetItemCount() <= 0)
	{
		BYTE bTestItemNum = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.bTestItemNum;

		for (BYTE i = 0; i < bTestItemNum; i++)
		{
			bUnitType[i] = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.tTestItemInfo[i].Type;
		}

		//�����б���
		list_allItem.InsertColumn(0, _T("block��"));
		list_allItem.SetColumnWidth(0, 75);

		/*CString strListTitle[15] = { _T("SLC ECC"), _T("TLC ECC"), _T("SLC ������"), _T("TLC ������"), _T("SLC д����"),
			_T("TLC д����"), _T("SLC ������"), _T("TLC ������"), _T("SLC ��ʱ��"), _T("TLC ��ʱ��"),
			_T("SLC дʱ��"), _T("TLC дʱ��"), _T("SLC ��ʱ��"), _T("TLC ��ʱ��"), _T("PECycle") };*/

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

	//����ļ�����С��8K���϶�����BBT�ļ�
	if (file.GetLength() < (8 * 1024))
	{
		DB_PRINT("\r\n[ReadBBTFile] �ļ�����С��8K!");
		MessageBox(strFilePath + _T("�ļ�����С��8K!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);

		file.Close();
		return;
	}

	//���BBTͷ��ǰ1K���ݣ�
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

	//�����ǰ��ȡ��BBT��¼�ļ�˳�����ʼ��λ�ã���֮ǰ����Ŀ���NULL����
	if (itemNumStart >= list_allItem.GetItemCount())
	{
		for (int i = list_allItem.GetItemCount(); i < itemNumStart; i++)
		{
			list_allItem.InsertItem(i, _T("nihao"));
			CString str;
			str.Format(_T("0x%04X"), i);
			list_allItem.SetItemText(i, 0, str);
			//���뻺��
			strListBuff[0].InsertAt(i, str);

			for (int j = 0; j < testItemNum; j++)
			{
				list_allItem.SetItemText(i, j + 1, _T("NULL"));
				//���뻺��
				strListBuff[j + 1].InsertAt(i, _T("NULL"));
			}
		}
	}

	//��ȡÿһ�����Ե�Ԫ���ֽڳ���
	BYTE bUnitByteLen[BLOCK_TEST_ITEMS_NUM_MAX] = { 0 };
	for (int i = 0; i < testItemNum; i++)
	{
		bUnitByteLen[i] = ((tyOfflineScanTableHead *)bReadBuff)->tBurninInfo.tResultUnitInfo.tTestItemInfo[i].ByteLen;
	}

	//�����ȡitem
	while (readSize = file.Read(bReadBuff, itemByteLen))
	{
		if (readSize != itemByteLen)
		{
			DB_PRINT("\r\n[ReadBBTFile] �ļ���ȡ���ִ���!");
			MessageBox(_T("�ļ���ȡ���ִ���!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
			break;
		}

		//�������У��������Ӧ��block��
		if (itemNum >= list_allItem.GetItemCount())
		{
			list_allItem.InsertItem(itemNum, _T("nihao"));
			CString str;
			str.Format(_T("0x%04X"), itemNum);
			list_allItem.SetItemText(itemNum, 0, str);
			//���뻺��
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

			//��������Unit���ݲ��뵽�����
			list_allItem.SetItemText(itemNum, i + 1, CString(bWriteBuff));
			//���뻺��
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

	//��������չʾ���б�ؼ���
	for (int j = 0; j < list_allItem.GetItemCount(); j++)
	{
		itemIndexMap[j] = j;
	}
	SetDataMode(cb_hexMode.GetCheck());

	file.Close();
}

void CBIBToolDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("�����ļ�(*.*)|*.*|");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter, this);
	CString strFilePath;

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//�������ͬʱ��20���ļ�
	fileDlg.m_ofn.lpstrFile = pBuffer;
	fileDlg.m_ofn.nMaxFile = MAX_PATH * 20;
	fileDlg.m_ofn.lpstrFile[0] = '\0';

	if (fileDlg.DoModal() == IDOK)
	{
		POSITION  pos = fileDlg.GetStartPosition();

		while (pos != NULL)
		{
			//�����ļ�·���Զ����ƶ�ȡ��ʽ���ļ�
			strFilePath = fileDlg.GetNextPathName(pos);
			LoadBBTFile(strFilePath);
		}
	}
}


void CBIBToolDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT count;
	TCHAR filePath[MAX_PATH] = { 0 };

	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//�ӳɹ����ϷŲ����м����ļ������ơ���ȡ������ק�ļ�����Ŀ
	
	for (int i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));//�����ק���ļ���
		LoadBBTFile(filePath);
	}
	UpdateData(FALSE);
	DragFinish(hDropInfo);//�Ϸųɹ����ͷ��ڴ�

	CDialogEx::OnDropFiles(hDropInfo);
}

void CBIBToolDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DB_PRINT("\r\n[Output cvs File]");
	CFile fileWrite;
	char bWriteBuff[1024];

	TCHAR szFilter[] = _T("���ݱ��ļ�(*.csv)|*.csv|�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("csv"), _T("my"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// ��ʾ�����ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{ 
		strFilePath = fileDlg.GetPathName();

		if (strFilePath.GetLength() <= 0)
		{
			DB_PRINT("\r\n[SaveFile]û��ѡ���ļ���!");
			return;
		}

		

		//������Ӧ���ļ�
		fileWrite.Open(strFilePath, CFile::modeWrite | CFile::modeRead | CFile::modeCreate);

		//д��BBTͷ��Ϣ
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

		sprintf_s(bWriteBuff, "block��,");
		fileWrite.Write(bWriteBuff, strlen(bWriteBuff));

		//д���б���
		for (int col = 0; bUnitType[col] != TEST_ITEM_NULL; col++)
		{
			sprintf_s(bWriteBuff, "%s,", bListTitle[bUnitType[col]]);
			fileWrite.Write(bWriteBuff, strlen(bWriteBuff));

		}
		fileWrite.Write("\n", 1);

		//����д��item
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int colSelect = pNMListView->iSubItem;//��õ�ǰ��ѡ��

	static BYTE bSortType = 1;  //0���������У�1����������

	//��ȡ������
	int lineTotal = list_allItem.GetItemCount();

	//strListBuff���ÿһ�����ݶ���˳�����еģ��������㷨ʱ��ȥ����buff������ݣ����ǽ�����������һ��ӳ��������
	for (int j = 0; j <= lineTotal; j++)
	{
		itemIndexMap[j] = j;
	}

	//ð������
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
	if (0 == bMode)  //ʮ����
	{
		//��������չʾ���б�ؼ���
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
	else  //ʮ������
	{
		//��������չʾ���б�ؼ���
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetDataMode(cb_hexMode.GetCheck());
}


void CBIBToolDlg::OnCbnSelchangeComboMsgSelect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	cbo_msgSelect.GetWindowTextW(str);
	
	edt_BBTHeadMsg.SetWindowTextW(strMsgSelect[wcstol(str.Mid(str.GetLength() - 1), NULL, 10)]);
	//edt_BBTHeadMsg.SetWindowTextW(str);
}


void CBIBToolDlg::OnBnClickedBtnClearList()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
