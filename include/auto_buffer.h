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
    explicit AutoBuffer();
    virtual ~AutoBuffer();


    virtual int Full() const;
    virtual int Empty() const;
    virtual size_t Size() const;

    virtual int ReadFromFd(int fd);
    virtual int ReadFromFd(int fd, size_t read_size);
    virtual int WriteToFd(int fd);
    virtual int WriteToFd(int fd, size_t write_size);
    
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
    static const int MAXBUFBLOCKSIZE = 2046;

    AutoBufferNode* InitBufNode(int size);
    void PushBufNode();
    void FreeBufNode(AutoBufferNode* buf_node);
    void PopBufNode();
};



#endif
