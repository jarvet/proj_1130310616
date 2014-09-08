#include "StreamBuffer.h"
#define N 30
class BufferPool
{
public:
    BufferPool(int num=10);
    ~BufferPool();
    void Setup(int num);//建立新的缓冲区
    char* Distribute(int id);//分配缓冲区
    void Recycle(int no, char* pData);//回收缓冲区
    void Release(int no);//释放缓冲区

private:
    int m_num;//缓冲区个数
    char *m_pData[N];//备用
    int m_toID[N];//被分配到的节目id
};
