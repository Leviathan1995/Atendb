#include "File.h"
#include <fstream>
//写入文件
bool Write(string & filename, string & content, int & num)
{
	ofstream Out(filename, ios::in | ios::out | ios::binary);	//  写追加到文件尾
	if (Out == 0)//如果文件不存在
	{
		//文件不存在，即创建文件
		File::CreateFile(filename);
		Out.open(filename, ios::in | ios::out | ios::binary);
	}
	Out.seekp(0, ios_base::end);
	long End = Out.tellp();
	num = (End / Block_Size);                                                                // 将(块)尾号作为传出参数
	string toBeWrite(Block_Size, 0);                                                     // ensure the rest of the 'toBeWrite' is 0
	toBeWrite = content;
	Out.seekp(num * Block_Size);
	Out.write(toBeWrite.c_str(), Block_Size);
	Out.close();

	return true;
}