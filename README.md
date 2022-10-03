# 轻量化的EEPROM空间管理程序

传统使用eeprom你需要**记住不同数据在eeprom片内的地址**

### 特点：

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

---

### 注意事项：

- 每个数据的大小**最大255个字节**
- 在写入数据时，你需要**按照枚举表中的顺序，从上往下依次写入**
- 一旦`variableLists`中的所有变量写入了`eeprom`，如果更改了variableLists中变量的次序，需要将eeprom**全部擦除**，**所有数据重新写入**
