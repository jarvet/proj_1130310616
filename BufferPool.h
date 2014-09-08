#include "StreamBuffer.h"
#define N 30
class BufferPool
{
public:
    BufferPool(int num=10);
    ~BufferPool();
    void Setup(int num);//�����µĻ�����
    char* Distribute(int id);//���仺����
    void Recycle(int no, char* pData);//���ջ�����
    void Release(int no);//�ͷŻ�����

private:
    int m_num;//����������
    char *m_pData[N];//����
    int m_toID[N];//�����䵽�Ľ�Ŀid
};
