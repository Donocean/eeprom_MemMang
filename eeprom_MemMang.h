#ifndef __EEPROM_MEMMANG_H_
#define __EEPROM_MEMMANG_H_

/* 用户自己编写eeprom的驱动文件 */
//#include "24cxx.h" 

/* 用户需将自己读写eeprom的函数填入下宏中 */
/* eeprom最大容量减一(以字节为单位)，如24C02宏大小为255 */
#define EE_MAXADDR 

/* 函数类型 void (*) (unsigned int eepromAddr, unsigned char* dataAddr, unsigned char num) */
#define eepromWirte 
#define eepromRead  

/* 想保存变量到eeprom时，首先在下面枚举中添加变量名 */
typedef enum
{
	// DATA0,DATA1只是示例，实际使用将17-21行全部删除
	DATA0,
	DATA1,
	// 用户将变量名添加到下面
	G_MYSENSORDATA,
	

	// DATA_NUM用于标识eeprom中一共存了多少变量(不允许删改)
	DATA_NUM,
}variableLists;



void readDataFromEEprom(void* dataAddr, variableLists dataId);
unsigned char writeDataToEEprom(void* dataAddr, unsigned char dataSize, variableLists dataId);
#endif /* __EEPROM_MEMMANG_H_ */

