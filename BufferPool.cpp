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
���ܣ����뽨���µĻ�����
������
	num���½��������ĸ���
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
���ܣ�����Ŀid����һ��������
������
	id :���뻺�����Ľ�Ŀid
����ֵ�����仺�����ĵ�ַ
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
���ܣ����ջ�����
������
	no�������������
	pData��ԭ��ָ�򱻻��ջ�������ָ��
*/
void BufferPool::Recycle(int no, char* pData)
{
    m_toID[no] = 0;
    pData = NULL;
}

/*
���ܣ��ͷŻ�����
������
	no�������������
*/
void BufferPool::Release(int no)
{
    m_toID[no] = 0;
    delete []m_pData[no];
}
