#include "BufferPool.h"

BufferPool::BufferPool(int num)
{
    m_num = num;
    for (int i=0; i<num; i++)
    {
        m_pData[i] = new char[512*1024];
        m_toID[i] = 0;
    }
}

BufferPool::~BufferPool()
{
    for (int i=0; i<m_num; i++)
        delete []m_pData[i];
}

/*
功能：申请建立新的缓冲区
参数：
	num：新建缓冲区的个数
*/
void BufferPool::Setup(int num)
{
    for ( ; m_num<num; m_num++)
    {
        m_pData[m_num] = new char[512*1024];
        m_toID[m_num] = 0;
    }

}

/*
功能：给节目id分配一个缓冲区
参数：
	id :申请缓冲区的节目id
返回值：分配缓冲区的地址
*/
char* BufferPool::Distribute(int id)
{
    for (int i=0; i<m_num; i++)
    {
        if (m_toID[i] == 0)
        {
            m_toID[i] = id;
            return m_pData[i];
        }
    }
    Setup(1);
    return m_pData[m_num];
}

/*
功能：回收缓冲区
参数：
	no：缓冲区的序号
	pData：原来指向被回收缓冲区的指针
*/
void BufferPool::Recycle(int no, char* pData)
{
    m_toID[no] = 0;
    pData = NULL;
}

/*
功能：释放缓冲区
参数：
	no：缓冲区的序号
*/
void BufferPool::Release(int no)
{
    m_toID[no] = 0;
    delete []m_pData[no];
}
