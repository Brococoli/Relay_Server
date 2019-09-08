#ifndef AUTO_BUFFER_H_
#define AUTO_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef UTILS_H_
#include "utils.h"
#endif

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#include <algorithm>
#define min(a,b) ((a)<(b)?(a):(b))

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
    AutoBuffer() ;
    virtual ~AutoBuffer();


    virtual int Full();
    virtual int Empty();
    virtual int ReadFromFd(int fd);
    virtual int WriteToFd(int fd);
    virtual int ReadFromCharArray(char* from, size_t size);
    virtual int WriteToCharArray(char* to, size_t size);

protected:
    virtual Buffer* Init(size_t size);
    virtual void Free();


private:
    typedef struct AutoBufferNode{
        char* data_ptr_;
        struct AutoBufferNode* next_;    
    }AutoBufferNode;

    size_t front_ptr_, rear_ptr_;
    AutoBufferNode *front_, *rear_;

    explicit AutoBuffer(size_t size);
    AutoBufferNode* InitBufNode(int size);
    void PushBufNode();
    void FreeBufNode(AutoBufferNode* buf_node);
    void PopBufNode();
};



#endif
