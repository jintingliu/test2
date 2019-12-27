/**
***************************************************************************
* 本文档定义了Table Head结构体参数，用于保存当前BBT的信息：eMMC sorting流程以及
* MP过程会用到
* 
* Table head位于每份BBT的第一个page，1份BBT的构成：HEAD + TABLEs，TABLEs的长度
* 在HEAD结构体中定义
*
***************************************************************************
* 版本更新说明：
* V1.0，2019.6.26，@wjw：	创建文件。定义 tyOfflineScanTableHead 结构体
* V1.1，2019.10.18，@ljt：	添加 TEST_ITEM_BLOCK_PE_CYCLE 宏
							添加 bWorkingVoltage 参数
							添加 bWorkingTemperature 参数
* V1.2，2019.11.04，@ljt：	删除dwSTM32Version
							添加dwSTM32CBVersion, dwSTM32TUVersion, dwSTM32RUVersion
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

#define SCAN_PRI_BLOCK_NUM_MAX			5		// 扫描最大私有数据块数量，扫描私有数据块包括：BBT block，Debug info block；这些block是单独扫描，只记录ECC，MP在遍历电流/性能测试结果时需跳过

//[TEST_COMPLETION]
#define TEST_COMPLETION_IS_NOT_DONE		0		//当前BBT的burnin test还没完成，需继续扫描
#define TEST_COMPLETION_IS_DONE			1		//当前BBT的Burnin test已经全部完成

//[PRI_BLOCK_IDX]
#define PRI_BLOCK_IDX_BBT				0		// BBT block
#define PRI_BLOCK_IDX_BBT_BACKUP		1		// BBT backup block
#define PRI_BLOCK_IDX_DEBUG_INFO		2		// 存放debug信息的block

#define BLOCK_TEST_ITEMS_NUM_MAX		32		// （4字节对齐）每个block的最大测试项数

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
	unsigned int   dwStructFlag;				// 结构体flag，见 OFFLINE_FLAG
	unsigned int   dwStructSize;				// 结构体尺寸，见 STRUCT_SIZE_TABLE_HEAD
	unsigned short wStructVersion;				// 结构体版本 OFFLINE_VER
	unsigned short wResultPageTotal;			// 存放结果表的page总数（包含HEAD）
	unsigned int   dwEmmcSortingVersion;		// 脱机扫描版本
	unsigned int   dwSTM32CBVersion;			// STM32端的CB通讯板软件版本号
	unsigned int   dwSTM32TUVersion;			// STM32端的TU测试单元软件版本号
	unsigned int   dwSTM32RUVersion;			// STM32端的RU结果记录单元软件版本号
	unsigned int   dwBIBPortNumber;				// BIB Port number
	
	struct
	{
		unsigned char  bTestCompletion;			// 扫描的完成情况，见 [TEST_COMPLETION]
		unsigned char  bWorkingVoltage;			// 工作电压，0：标压，1：低压，2：高压
		unsigned char  bWorkingTemperature;		// 工作温度
		unsigned char  bReserve0[3];			// 对齐保留
		unsigned short wSlcBurninVccqMax;		// Burn-in SLC 阶段VCCQ最大值
		unsigned short wTlcBurninVccqMax;		// Burn-in TLC 阶段VCCQ最大值
		unsigned short wVDTCnt;					// 低电压异常记录
		unsigned int   dwDuration;				// 测试时间（内部定时器），单位：ms

		unsigned short wSectionNum;				// 分段号
		unsigned short wLastSectionPos;			// 上一个分段所在的位置
		unsigned short wSectionTotal;			// 分段总数
		unsigned short wSectionStartBlockNum;	// 当前分段的起始block号
		unsigned short wSectionBlockTotal;		// 当前分段的测试block数
		unsigned char  bReserve1[2];			// 对齐保留

		struct
		{
			unsigned char bTestItemNum;			// 最大值为：BLOCK_TEST_ITEMS_NUM_MAX
			unsigned char bBUByteLen;			// 1个测试结果的长度 BYTE
			unsigned char bReserve2[2];			// 对齐保留
			unsigned int  dwTestItemDoneBit;	// 测试项完成后对应bit置1
		
			struct								// 测试项结果信息，指明每个block测试结果单元的测试类型以及结果长度
			{
				unsigned char Type : 4;			// 测试结果类型，见 [TEST_ITEM_INFO_TYPE]
				unsigned char ByteLen : 4;		// 占用字节数
			} tTestItemInfo[BLOCK_TEST_ITEMS_NUM_MAX];
		}tResultUnitInfo;

		struct
		{
			unsigned short wBlockNum;
			unsigned char  bChipNum;
			unsigned char  bPECycle;
		} tScanPriBlk[SCAN_PRI_BLOCK_NUM_MAX];	// 扫描私有数据块，每个block定义见 [PRI_BLOCK_IDX]
		
		struct
		{
			unsigned int dwStart;
			unsigned int dwEnd;
			unsigned int dwMax;
			unsigned int dwMin;
		} tTemperatureValue;					// 测试过程环境温度记录

		unsigned char bProcessData[128];		// 流程数据，用于上电恢复扫描进度
	} tBurninInfo;

	unsigned char bBurninStartupParam[512];
	unsigned char bReserve[244];				// 保留, 0(default)
	unsigned int  dwChecksum;					// 结构体异或值

} tyOfflineScanTableHead;

#define STRUCT_SIZE_CHECK_TABLEHEAD		(sizeof(tyOfflineScanTableHead))		//该值需等于 STRUCT_SIZE_TABLE_HEAD

#endif
