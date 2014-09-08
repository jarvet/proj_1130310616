#include "Program.h"
#include <string.h>

Program::Program(int id)
{
    m_id = id;
    l_buffer = new BufferList;
    l_buffer->buff = NULL;
    l_buffer->next = NULL;
}

Program::~Program()
{
    delete l_buffer;
}

 /*
���ܣ����ղ������ݴ��뻺����
������
    offset���������ļ��е�ƫ����
    pData��ָ�����ݵ�ָ��
*/
void Program::Receive(unsigned int offset, char *pData)
{
    if (l_buffer->buff == NULL)//��һ�Σ���ʼ�������
    {
        l_buffer->buff = new StreamBuffer;
        l_buffer->next = NULL;
        l_buffer->buff->ReceiveDate(offset, strlen(pData), pData);
    }
    else
    {
        BufferList *temp;//����Ѱ�Һ��ʵĻ�����
        while (temp->buff != NULL)
        {
            if (offset >= temp->buff->GetOffset()
            && offset+strlen(pData) < temp->buff->GetOffset()+temp->buff->GetOffset())
            {//��ǰ�������ܹ�����������
                temp->buff->ReceiveDate(offset, strlen(pData), pData);
                break;
            }
            else
            {
                if (temp->next != NULL)
                    temp = temp->next;
                else//�����һ����������δ�ҵ����ʵģ��򴴽�һ���µĻ�����
                {
                    temp->next = new BufferList;
                    temp = temp->next;
                    temp->buff = new StreamBuffer;
                    temp->buff->ReceiveDate(offset, strlen(pData), pData);
                    temp->next = NULL;
                    break;
                }//end for if
            }//end for if
        }//end for while
    }//end for if
}

/*
���ܣ�������д���ļ�
������Ҫд����ļ���ַ
*/
void Program::Write(FILE *fpDstFile)
{//��λ�������ԭ���Ŀ��д������������������������ݺ�ɾ��
    char *pOutData;
	unsigned int iOutDataOffset;
	int iContinueBytes;
	int iUseBytes;

	BufferList *temp = l_buffer;
	while (temp->buff != NULL)
	{
		iContinueBytes = temp->buff->ContinueBytes(iOutDataOffset, pOutData);
        iUseBytes = iContinueBytes - 100;//��������һ����
		if (iContinueBytes > 1024) //ʾ����ֵ�����Ե���
		{
			fseek(fpDstFile, iOutDataOffset, SEEK_SET);
			fwrite(pOutData, iUseBytes, 1, fpDstFile);
			temp->buff->RemoveData(iUseBytes);
		}
		temp = temp->next;
	}
}

/*
���ܣ�����������������ʣ���ļ�д���ļ���ɾ��������
������Ҫд����ļ���ַ
*/
void Program::Clean(FILE *fpDstFile)
{
    BufferList *temp = l_buffer;

    while (temp->buff != NULL)
    {
        temp->buff->ClearData(fpDstFile);//��������
        delete temp->buff;//ɾ���ڵ�
        temp = temp->next;
    }
}
