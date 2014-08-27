#if !defined  SREAM_BUFFER_H_HIT_CPP
#define SREAM_BUFFER_H_HIT_CPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <queue>
#include <utility>
using namespace std;

class StreamBuffer
{
 public:
    StreamBuffer();
    StreamBuffer(int iLen);
    ~StreamBuffer();
	int ReceiveDate(unsigned int offset, unsigned int bytes, char *pData);//接收数据
	int ContinueBytes(unsigned int &iDataOffset, char* &pData);//获取缓冲区中排好序的数据长度、第一个字节的偏移量和数据指针
	int RemoveData(int iBytes);//删除 用过的数据
	void ClearData(FILE *fpDstFile);//排空剩余数据
	unsigned int GetOffset();//用来给program获取缓冲区当前在文件中的偏移量
	int GetBufferLen();//用来给program获取缓冲区总长度

 private:
    char *m_pData;//存数据的buffer，也可以用指针
    int m_iBufferLen;//缓冲区长度
    unsigned int m_offset;//缓冲区数据在文件中的偏移量
    char *head, *tail;  // head、tail为当前连续数据的首尾指针
    priority_queue <pair <unsigned int, unsigned int>, vector<pair <unsigned int, unsigned int> >, greater<pair <unsigned int, unsigned int> > > packInfo;
    //跟大神学的……
    // 使用优先队列记录数据包信息，队列中元素为pair型
    // pair中两个数据分别为数据包偏移量、数据包大小
    // 数据包偏移量最小的数据始终在队首(优先队列特点)
};

#endif
