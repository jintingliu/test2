/**
***************************************************************************
* ���ĵ�������Table Head�ṹ����������ڱ��浱ǰBBT����Ϣ��eMMC sorting�����Լ�
* MP���̻��õ�
* 
* Table headλ��ÿ��BBT�ĵ�һ��page��1��BBT�Ĺ��ɣ�HEAD + TABLEs��TABLEs�ĳ���
* ��HEAD�ṹ���ж���
*
***************************************************************************
* �汾����˵����
* V1.0��2019.6.26��@wjw��	�����ļ������� tyOfflineScanTableHead �ṹ��
* V1.1��2019.10.18��@ljt��	��� TEST_ITEM_BLOCK_PE_CYCLE ��
							��� bWorkingVoltage ����
							��� bWorkingTemperature ����
* V1.2��2019.11.04��@ljt��	ɾ��dwSTM32Version
							���dwSTM32CBVersion, dwSTM32TUVersion, dwSTM32RUVersion
***************************************************************************
*@copy
*    
* <h2><center>&copy; COPYRIGHT 2019 Longsys</center></h2>
***************************************************************************
*/

#ifndef _TABLE_HEAD_H_
#define _TABLE_HEAD_H_

#define OFFLINE_FLAG					0x6699af0c
#define OFFLINE_VER						0x0102
#define STRUCT_SIZE_TABLE_HEAD			1024

#define SCAN_PRI_BLOCK_NUM_MAX			5		// ɨ�����˽�����ݿ�������ɨ��˽�����ݿ������BBT block��Debug info block����Щblock�ǵ���ɨ�裬ֻ��¼ECC��MP�ڱ�������/���ܲ��Խ��ʱ������

//[TEST_COMPLETION]
#define TEST_COMPLETION_IS_NOT_DONE		0		//��ǰBBT��burnin test��û��ɣ������ɨ��
#define TEST_COMPLETION_IS_DONE			1		//��ǰBBT��Burnin test�Ѿ�ȫ�����

//[PRI_BLOCK_IDX]
#define PRI_BLOCK_IDX_BBT				0		// BBT block
#define PRI_BLOCK_IDX_BBT_BACKUP		1		// BBT backup block
#define PRI_BLOCK_IDX_DEBUG_INFO		2		// ���debug��Ϣ��block

#define BLOCK_TEST_ITEMS_NUM_MAX		32		// ��4�ֽڶ��룩ÿ��block������������

//[TEST_ITEM_INFO_TYPE]
#define TEST_ITEM_BBT_SLC				0
#define TEST_ITEM_BBT_TLC				1
#define TEST_ITEM_AP_ERASE_SLC			2
#define TEST_ITEM_AP_ERASE_TLC			3
#define TEST_ITEM_AP_PROGRAM_SLC		4
#define TEST_ITEM_AP_PROGRAM_TLC		5
#define TEST_ITEM_AP_READ_SLC			6
#define TEST_ITEM_AP_READ_TLC			7
#define TEST_ITEM_tERASE_SLC			8
#define TEST_ITEM_tERASE_TLC			9
#define TEST_ITEM_tPROGRAM_SLC			10
#define TEST_ITEM_tPROGRAM_TLC			11
#define TEST_ITEM_tREAD_SLC				12
#define TEST_ITEM_tREAD_TLC				13
#define TEST_ITEM_BLOCK_PE_CYCLE		14
#define TEST_ITEM_NULL					0xFF

typedef struct
{
	unsigned int   dwStructFlag;				// �ṹ��flag���� OFFLINE_FLAG
	unsigned int   dwStructSize;				// �ṹ��ߴ磬�� STRUCT_SIZE_TABLE_HEAD
	unsigned short wStructVersion;				// �ṹ��汾 OFFLINE_VER
	unsigned short wResultPageTotal;			// ��Ž�����page����������HEAD��
	unsigned int   dwEmmcSortingVersion;		// �ѻ�ɨ��汾
	unsigned int   dwSTM32CBVersion;			// STM32�˵�CBͨѶ������汾��
	unsigned int   dwSTM32TUVersion;			// STM32�˵�TU���Ե�Ԫ����汾��
	unsigned int   dwSTM32RUVersion;			// STM32�˵�RU�����¼��Ԫ����汾��
	unsigned int   dwBIBPortNumber;				// BIB Port number
	
	struct
	{
		unsigned char  bTestCompletion;			// ɨ������������� [TEST_COMPLETION]
		unsigned char  bWorkingVoltage;			// ������ѹ��0����ѹ��1����ѹ��2����ѹ
		unsigned char  bWorkingTemperature;		// �����¶�
		unsigned char  bReserve0[3];			// ���뱣��
		unsigned short wSlcBurninVccqMax;		// Burn-in SLC �׶�VCCQ���ֵ
		unsigned short wTlcBurninVccqMax;		// Burn-in TLC �׶�VCCQ���ֵ
		unsigned short wVDTCnt;					// �͵�ѹ�쳣��¼
		unsigned int   dwDuration;				// ����ʱ�䣨�ڲ���ʱ��������λ��ms

		unsigned short wSectionNum;				// �ֶκ�
		unsigned short wLastSectionPos;			// ��һ���ֶ����ڵ�λ��
		unsigned short wSectionTotal;			// �ֶ�����
		unsigned short wSectionStartBlockNum;	// ��ǰ�ֶε���ʼblock��
		unsigned short wSectionBlockTotal;		// ��ǰ�ֶεĲ���block��
		unsigned char  bReserve1[2];			// ���뱣��

		struct
		{
			unsigned char bTestItemNum;			// ���ֵΪ��BLOCK_TEST_ITEMS_NUM_MAX
			unsigned char bBUByteLen;			// 1�����Խ���ĳ��� BYTE
			unsigned char bReserve2[2];			// ���뱣��
			unsigned int  dwTestItemDoneBit;	// ��������ɺ��Ӧbit��1
		
			struct								// ����������Ϣ��ָ��ÿ��block���Խ����Ԫ�Ĳ��������Լ��������
			{
				unsigned char Type : 4;			// ���Խ�����ͣ��� [TEST_ITEM_INFO_TYPE]
				unsigned char ByteLen : 4;		// ռ���ֽ���
			} tTestItemInfo[BLOCK_TEST_ITEMS_NUM_MAX];
		}tResultUnitInfo;

		struct
		{
			unsigned short wBlockNum;
			unsigned char  bChipNum;
			unsigned char  bPECycle;
		} tScanPriBlk[SCAN_PRI_BLOCK_NUM_MAX];	// ɨ��˽�����ݿ飬ÿ��block����� [PRI_BLOCK_IDX]
		
		struct
		{
			unsigned int dwStart;
			unsigned int dwEnd;
			unsigned int dwMax;
			unsigned int dwMin;
		} tTemperatureValue;					// ���Թ��̻����¶ȼ�¼

		unsigned char bProcessData[128];		// �������ݣ������ϵ�ָ�ɨ�����
	} tBurninInfo;

	unsigned char bBurninStartupParam[512];
	unsigned char bReserve[244];				// ����, 0(default)
	unsigned int  dwChecksum;					// �ṹ�����ֵ

} tyOfflineScanTableHead;

#define STRUCT_SIZE_CHECK_TABLEHEAD		(sizeof(tyOfflineScanTableHead))		//��ֵ����� STRUCT_SIZE_TABLE_HEAD

#endif
