#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "StreamBuffer.h"// for Code::Block

using namespace std;
//-------------------
 StreamBuffer::StreamBuffer()
 {
 //  to do：  some initialization work
   m_pData = head = tail = new char[512*1024];
   m_iBufferLen = 512*1024;
   m_offset = 0;

 }
  StreamBuffer::StreamBuffer(int iLen)
 {
 //  to do：  some initialization work
   m_pData =head = tail =  new char[iLen];
   m_iBufferLen = iLen;
   m_offset = 0;
 }
StreamBuffer::~StreamBuffer()
 {
 //  to do：  some initialization work
  if( NULL !=  m_pData)
   delete []m_pData;

 }

 /*
功能：接收并将数据存入缓冲区
参数：
    offset：数据在文件中的偏移量
    bytes：存入的数据字节数
    pData：指向数据的指针
返回值：存入数据的字节数
*/
int StreamBuffer::ReceiveDate(unsigned int offset, unsigned int bytes, char *pData)
{

    unsigned int iBytes = 0;

    //若缓冲区空间不够则将数据前移，因为此时认为head之前数据已经排空，故不涉及丢包
    if (offset + bytes > m_offset + m_iBufferLen)
    {
        memcpy(m_pData, head, m_iBufferLen - (head - m_pData));
        tail -= head - m_pData;
        m_offset += head - m_pData;//修正偏移量
        head = m_pData;
    }

    if (offset<m_offset)//更正实验二中的错误，offset<m_offset时把数据放在缓冲区头部
    {
        for (int i=m_iBufferLen-bytes; i>=0; i--)
            m_pData[i+bytes] = m_pData[i];
        memcpy(m_pData,pData,bytes);

        head = m_pData;
        tail = m_pData + bytes;

        packInfo.push(make_pair(offset,bytes));
    }
    else
    {
        for ( ; iBytes<bytes; iBytes++)
            m_pData[iBytes+offset-m_offset] = pData[iBytes];//放入缓冲区

        packInfo.push(make_pair(offset, bytes));    // 将数据包信息存入优先队列

        if (bytes+offset-m_offset > m_iBufferLen*0.8)//超过缓存区一定部分而头部连续数据还未写入就放弃
        {
            //输出丢包信息，依然是head之前已经排空所以只有head和下一个head之间算是丢包
            ofstream errlog("err.log",ios::app);
            errlog << m_offset+head-m_pData << " to " << packInfo.top().first << " was lost!" << endl;

            head = m_pData - m_offset + packInfo. top().first;//下一段连续数据的开始位置
            tail = head + packInfo.top().second;//下一段连续数据的末尾
            packInfo.pop();//弹出队首元素
        }

    }

   return iBytes;// bytes the buffer saved
}

/*
功能：获取缓冲区中排好序的数据长度、第一个字节的偏移量和数据指针
参数：
    iDataOffset：用于返回排好序的数据块中第一个字节的偏移量数值
    pData:数据指针
返回值：缓冲区中排好序的数据长度
*/
int StreamBuffer::ContinueBytes(unsigned int &iDataOffset, char* &pData)
{
  int iContinueBytes = 0;

    // 优先队列的队首元素记录队列中偏移量最小的数据包信息
    // 当前连续部分和优先队列队首所记录的数据包连续时，更新当前连续部分，并弹出队首元素
    while ((m_offset + (tail - m_pData)) == packInfo.top().first)
    {
        tail += packInfo.top().second;
        packInfo.pop();
    }

    iDataOffset = m_offset + (head - m_pData);//修整偏移量
    pData = head;

    iContinueBytes = tail-head;//连续部分长度，即首尾指针指向的长度

  return iContinueBytes;
};

/*
功能：从缓冲区中删除数据
参数：
    iBytes：删除的字节数
返回值：删除的字节数
*/
int StreamBuffer::RemoveData(int iBytes)
{
  int iBytesRemoved=0;

    //从缓冲区中把数据"删除"

    iBytesRemoved=iBytes;
    head += iBytes;

   return iBytesRemoved;
};

/*
功能：排空剩余数据
参数：指向目标文件的指针fpDstFile
*/
void StreamBuffer::ClearData(FILE *fpDstFile)
{
    while (!packInfo.empty())
    {
       //输出丢包信息
        ofstream errlog("err.log",ios::app);
        errlog << m_offset+head-m_pData << " to " << packInfo.top().first << " was lost!" << endl;

       head = m_pData - m_offset + packInfo. top().first;//剩余连续数据的开始位置
       fseek(fpDstFile,packInfo.top().first,SEEK_SET);
       fwrite(head,packInfo.top().second,1,fpDstFile);
       packInfo.pop();

    }
}

/*
返回缓冲区当前在文件中的偏移量
*/
unsigned int StreamBuffer::GetOffset()
{
    return m_offset;
}

/*
返回缓冲区总长度
*/
int StreamBuffer::GetBufferLen()
{
    return m_iBufferLen;
}
