#include "program.h"
#include <string.h>
program::program(int id)
{
    m_id = id;
    l_buffer = new BufferList;
    l_buffer->buff = NULL;
    l_buffer->next = NULL;
}

program::~program()
{
    delete l_buffer;
}

 /*
功能：接收并将数据存入缓冲区
参数：
    offset：数据在文件中的偏移量
    pData：指向数据的指针
*/
void program::Receive(unsigned int offset, char *pData)
{
    if (l_buffer->buff == NULL)//添一段，初始的情况下
    {
        l_buffer->buff = new StreamBuffer;
        //
        //
        l_buffer->next = NULL;
        l_buffer->buff->ReceiveDate(offset, strlen(pData), pData);
    }
    else
    {
        BufferList *temp;//用来寻找合适的缓冲区
        while (temp->buff != NULL)
        {
            if (offset >= temp->buff->GetOffset()
            && offset+strlen(pData) < temp->buff->GetOffset()+temp->buff->GetOffset())
            {//当前缓冲区能够容乃下数据
                temp->buff->ReceiveDate(offset, strlen(pData), pData);
                break;
            }
            else
            {
                if (temp->next != NULL)
                    temp = temp->next;
                else//到最后一个缓冲区仍未找到合适的，则创建一个新的缓冲区
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
功能：将数据写入文件
参数：要写入的文件地址
*/
void program::Write(FILE *fpDstFile)
{//这段基本照着原来的框架写的……
    char *pOutData;
	unsigned int iOutDataOffset;
	int iContinueBytes;
	int iUseBytes;

	BufferList *temp = l_buffer;
	while (temp->buff != NULL)
	{
		iContinueBytes = temp->buff->ContinueBytes(iOutDataOffset, pOutData);
        iUseBytes = iContinueBytes - 100;//假设用了一部分
		if (iContinueBytes > 1024) //示例数值，可以调整
		{
			fseek(fpDstFile, iOutDataOffset, SEEK_SET);
			fwrite(pOutData, iUseBytes, 1, fpDstFile);
			temp->buff->RemoveData(iUseBytes);
		}
		temp = temp->next;
	}
}

/*
功能：清理工作，将缓冲区剩余文件写入文件并删除缓冲区
参数：要写入的文件地址
*/
void program::Clean(FILE *fpDstFile)
{
    BufferList *temp = l_buffer;

    while (temp->buff != NULL)
    {
        temp->buff->ClearData(fpDstFile);//清理缓冲区
        delete temp->buff;//删除节点
        temp = temp->next;
    }
}
