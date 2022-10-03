# 轻量化的EEPROM空间管理程序

传统使用eeprom你需要**记住不同数据在eeprom片内的地址**

### 本程序特点：

- 简单易用，只有**两个API函数**
	- `void readDataFromEEprom(void* dataAddr, variableLists dataId)`:读数据
	- `unsigned char writeDataToEEprom(void* dataAddr, unsigned char dataSize, variableLists dataId)`: 写数据
- 容易维护，你只需要维护一个枚举变量表`variableLists`，通过此表读写eeprom中的数据
- 空间损失少，每个变量在eeprom中**仅多占用了1字节**

---

### 如何开始使用？

首先进入头文件，根据注释将**三个宏**填写完整
- `EE_MAXADDR`:  当前型号的eeprom容量大小减一
- `eepromWirte`: eeprom写操作驱动函数名
- `eepromRead`:  eeprom读操作驱动函数名

应用实例：将变量`float g_mySensorData = 3.14`通过api函数写入eeprom

 首先先在头文件添加变量名，用于管理变量
```c
typedef enum
{
	// 用户将变量名添加到下面
	G_MYSENSORDATA,
	

	// DATA_NUM用于标识eeprom中一共存了多少变量(不允许删改)
	DATA_NUM,
}variableLists;
```

使用API函数
```c
float g_mySensorData = 3.14;

void func(void)
{
	float tmp = 0;

	// 将数据写入eeprom
	writeDataToEEprom((void*)&g_mySensorData, sizeof(g_mySensorData), G_MYSENSORDATA);

	// 从eeprom中读取数据
	readDataFromEEprom((void*)&tmp, G_MYSENSORDATA);
}
```

---

### 注意事项：

- 每个数据的大小**最大255个字节**
- 在写入数据时，你需要**按照枚举表中的顺序，从上往下依次写入**
- 一旦`variableLists`中的所有变量写入了`eeprom`，如果更改了variableLists中变量的次序，需要将eeprom**全部擦除**，**所有数据重新写入**
