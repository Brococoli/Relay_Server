#ifndef AUTO_BUFFER_H_
#define AUTO_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "buffer.h"


/*---------------------------------------*/
/*
 *front                     rear
 *  |                        |
 *-----  ------  ------   ----- 
 *|---|->|----|->|----|->|-----|
 *-----  ------  ------  ------
 *  |                     | 
 * front_ptr             rear_ptr
 * 
 */


class AutoBuffer: public Buffer
{
public:
    explicit AutoBuffer(size_t size = MAXBUFSIZE);
    virtual ~AutoBuffer();


    virtual int Full();
    virtual int Empty();
    virtual int ReadFromFd(int fd);
    virtual int ReadFromFd(int fd, int read_size);
    virtual int WriteToFd(int fd);
    virtual int ReadFromCharArray(char* from, size_t size);
    virtual int WriteToCharArray(char* to, size_t size);
    
    virtual int Clear();

protected:
    virtual void Init(size_t size);
    virtual void Free();


private:
    typedef struct AutoBufferNode{
        char* data_ptr_;
        struct AutoBufferNode* next_;    
    }AutoBufferNode;

    size_t front_ptr_, rear_ptr_;
    AutoBufferNode *front_, *rear_;

    AutoBufferNode* InitBufNode(int size);
    void PushBufNode();
    void FreeBufNode(AutoBufferNode* buf_node);
    void PopBufNode();
};



#endif
