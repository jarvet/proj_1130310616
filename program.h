#include <string.h>
#include "StreamBuffer.h"

struct BufferList
{
    StreamBuffer *buff;
    BufferList *next;
};

class Program
{
public:
    Program(int id=0);
    ~Program();
    void Receive(unsigned int offset, char *pData);//节目接收数据
    void Write(FILE *fpDstFile); //将数据写入文件
    void Clean(FILE *fpDstFile);//清理工作，将缓冲区剩余文件写入文件并删除缓冲区

private:
    BufferList *l_buffer;
    int m_id;
};
