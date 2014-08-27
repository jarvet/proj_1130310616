#include <string.h>
#include "StreamBuffer.h"
struct BufferList
{
    StreamBuffer *buff;

    BufferList *next;
};

class program
{
public:
    program(int id=0);
    ~program();
    void Receive(unsigned int offset, char *pData);//��Ŀ��������
    void Write(FILE *fpDstFile); //������д���ļ�
    void Clean(FILE *fpDstFile);//����������������ʣ���ļ�д���ļ���ɾ��������

private:
    BufferList *l_buffer;
    int m_id;
};
