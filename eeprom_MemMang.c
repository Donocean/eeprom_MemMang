/**
 * @file eeprom_MemMang.c
 * @author Donocean (1184427366@qq.com)
 * @brief 轻量化的用于eeprom的空间管理
 * @version 1.0
 * @date 2022-010-3
 * 
 * @copyright Copyright (c) 2022, Donocean
 */

#include "eeprom_MemMang.h"
#include <stdlib.h>
#include <stddef.h>

/* ---------------eeprom空间管理分配图------------------- */
/* |------------------------------+---------------------| */
/* | EEPROM片内地址               | 地址中保存的数据    | */
/* |------------------------------+---------------------| */
/* | 0x00                         | 保存data0的数据     | */
/* |------------------------------+---------------------| */
/* | 0x00 + data0的数据大小       | 保存data1的数据     | */
/* |------------------------------+---------------------| */
/* | ...                          |                     | */
/* |------------------------------+---------------------| */
/* | EE_MAXADDR - DATA1           | 保存data1的数据大小 | */
/* |------------------------------+---------------------| */
/* | EE_MAXADDR - DATA0(枚举类型) | 保存data0的数据大小 | */
/* |------------------------------+---------------------| */

/**
 * @brief: 保存一个数据到eeprom
 * @param: dataAddr 数据的地址
 * @param: dataSize 数据的大小(可取范围1-255个字节)
 * @param: dataId   可取范围见.h文件variableLists枚举定义
 * @usage: 想将 int g_mySensorData; 存储到eeprom
 *         1. 首先在枚举变量中加入变量名，如头文件所示
 *         2. 然后调用writeDataToEEprom((void*)&g_mySensorData, sizeof(g_mySensorData), G_MYSENSORDATA);
 * @retval: 0 写入成功 
 *          1 写入失败 
 * 失败的原因:
 *			1. 写入的数据大小为0
 *			2. 当前写入的数据之前(variableLists)里有数据没有写入
 *			3. eeprom剩余空间不够容纳当前写入的数据
 *			4. 堆申请失败
 * @warning: 因为数据的偏移地址是根据每个数据大小决定的，因此写入时必须按照variableLists中的顺序从上往下依次写入。
 */
e_uint8 writeDataToEEprom(void* dataAddr, e_uint8 dataSize, variableLists dataId)
{
	e_uint32 i;
	e_uint8* tmp = NULL;
	e_uint32 eeDataAddr = 0;
	e_uint8 lastDataSize = 0, currentDataSize = 0;
	
	if(dataSize == 0) return 1;

	/* 不能是第一个数据 */
	if(dataId != 0)
	{
		/* 写入前，先检查上一个地址有没有数据写入 */
		eepromRead((e_uint32)(EE_MAXADDR + dataId - 1), &lastDataSize, 1);

		if(lastDataSize == 0)
		{
			return 1;
		}
		
		/* 计算数据写入地址 */
		tmp = (e_uint8*)malloc(dataId);

		if(tmp != NULL)
		{
			eepromRead(EE_MAXADDR - (dataId - 1), tmp, dataId);

			/* 计算写入变量的实际地址 */
			for(i = 0; i < dataId; i++)
			{
				eeDataAddr += tmp[i];
			}

			free(tmp);
		}
		else 
		{
			/* 堆申请失败返回*/
			return 1;
		}
	}

	/* 若写入的数据将要覆盖eeprom中的variableLists，直接退出 */
	if((eeDataAddr + dataSize) > (EE_MAXADDR - dataId))
	{
		return 1;
	}

	/* 获得当前数据大小，如果为0说明是第一次写入 */
	eepromRead((e_uint32)(EE_MAXADDR - dataId), &currentDataSize, 1);

	if(currentDataSize == 0)
	{
		/* 将当前数据大小写入eeprom中的variableLists */
		eepromWirte((e_uint32)(EE_MAXADDR - dataId), &dataSize, 1);
	}
	
	/* 写入数据 */
	eepromWirte(eeDataAddr, (e_uint8*)dataAddr, dataSize);
	
	return 0;
}


/**
 * @brief: 保存一个变量到eeprom
 * @param: dataAddr 数据的地址
 * @param: dataId   可取范围见.h文件枚举定义
 * @usage: 想将 int g_mySensorData; 从eeprom，读取到单片机
 *		   调用readDataFromEEprom((void*)&g_mySensorData, G_MYSENSORDATA)
 */
void readDataFromEEprom(void* dataAddr, variableLists dataId)
{
	e_uint32 i;
	e_uint8* tmp = NULL;
	e_uint8 dataSize = 0;
	e_uint32 eeDataAddr = 0;
	
	/* 获得当前数据大小 */
	eepromRead((e_uint32)(EE_MAXADDR - dataId), &dataSize, 1);

	/* 申请地址(不能是第一个数据) */
	if(dataId != 0)
	{
		tmp = (e_uint8*)malloc(dataId);

		eepromRead(EE_MAXADDR - (dataId - 1), tmp, dataId);

		/* 计算读取变量的实际地址 */
		for(i = 0; i < dataId; i++)
		{
			eeDataAddr += tmp[i];
		}

		free(tmp);
	}

	/* 读取数据 */
	eepromRead(eeDataAddr, (e_uint8*)dataAddr, dataSize);
}

