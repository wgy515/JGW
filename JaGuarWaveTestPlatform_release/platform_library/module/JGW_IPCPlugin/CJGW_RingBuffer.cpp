#include "StdAfx.h"
#include "CJGW_RingBuffer.h"
//#define min(a, b) ((a) < (b) ? (a) : (b))
namespace JGW
{
    CCJGW_RingBuffer::CCJGW_RingBuffer(void) : mpDataBuffer(NULL),mbIsSelfCreateBuffer(false)
    {
    }


    CCJGW_RingBuffer::~CCJGW_RingBuffer(void)
    {
        if (mbIsSelfCreateBuffer)
        {
            delete mpRingBuffer_t;
            mpRingBuffer_t = NULL;

            delete[] mpDataBuffer;
            mpDataBuffer = NULL;
        }
    }

    // 判断 val 值是否为 2 的幂
    static inline char is_power_of_two(unsigned int val)
    {
        if (val < 2)
            return 0;

        // 假设 val 值为 8，那么 1000 & 0111 == 0 为真
        return (val & (val - 1)) == 0;
    }

    // 向上获取距离 val 值最近的 2 的幂（val 本身不能是 2 的幂）
    static inline unsigned int roundup_power_of_two(unsigned int val)
    {
        if (val == 0)
            return 2;

        int bits = 0; // val 的二进制位数
        for (; val != 0; bits++)
            val >>= 1;

        return 1U << bits; // 返回比 val 位数多一位的 2 的幂
    }

    bool CCJGW_RingBuffer::CreateRingBuffer(size_t nBufferLen)
    {
        if (0 == is_power_of_two(nBufferLen))
            nBufferLen = roundup_power_of_two(nBufferLen);

        mpRingBuffer_t = new ringbuffer_t;
        mpRingBuffer_t->rpos = 0;
        mpRingBuffer_t->size = 0;
        mpRingBuffer_t->wpos = 0;
        mpDataBuffer = new unsigned char[nBufferLen];
        mbIsSelfCreateBuffer = true;

        return true;
    }

    bool CCJGW_RingBuffer::CreateRingBuffer(ringbuffer_t* pRingBuffer_t,void* pBuffer,size_t nBufferLen)
    {
        //! 环形缓冲区信息不能为空，以及缓冲区大小必须是2的冥
        if (NULL == pRingBuffer_t || NULL == pBuffer || 0 == is_power_of_two(nBufferLen)) return false;

        mpRingBuffer_t = pRingBuffer_t;
        mpDataBuffer = (unsigned char*)pBuffer;
        mpRingBuffer_t->rpos = mpRingBuffer_t->wpos = 0;
        mpRingBuffer_t->size = nBufferLen;

        return true;
    }

    bool CCJGW_RingBuffer::CreateRingBuffer(ringbuffer_t* pRingBuffer_t,void* pBuffer)
    {
        if (NULL == pRingBuffer_t || NULL == pBuffer) return false;

        mpRingBuffer_t = pRingBuffer_t;
        mpDataBuffer = (unsigned char*)pBuffer;

        return true;
    }

    int CCJGW_RingBuffer::WriteRingBuffer(unsigned char *data, unsigned int len)
    {
        _ASSERT(mpRingBuffer_t && data);

        // 剩余空间不足
        if (len > mpRingBuffer_t->size - (mpRingBuffer_t->wpos - mpRingBuffer_t->rpos)) 
            return -1;

        // 写索引
        unsigned int widx = mpRingBuffer_t->wpos & (mpRingBuffer_t->size - 1);
        // 第 1 次写入长度
        unsigned int wlen = min(len, mpRingBuffer_t->size - widx);
        // 写入写索引后面空间
        memcpy(mpDataBuffer + widx, data, wlen);        
        // 写入缓冲区起始空间
        if (wlen < len)                                    
            memcpy(mpDataBuffer, data + wlen, len - wlen);

        mpRingBuffer_t->wpos += len;
        return 0;
    }

    unsigned int CCJGW_RingBuffer::ReadRingBuffer(unsigned char *data, unsigned int size)
    {
        _ASSERT(mpRingBuffer_t && data);

        // 实际读取数据长度
        unsigned int len = min(size, mpRingBuffer_t->wpos - mpRingBuffer_t->rpos);
        // 读索引
        unsigned int ridx = mpRingBuffer_t->rpos & (mpRingBuffer_t->size - 1);   
        // 第 1 次读取长度
        unsigned int rlen = min(len, mpRingBuffer_t->size - ridx); 
        // 从读索引后面空间读取
        memcpy(data, mpDataBuffer + ridx, rlen);           
        // 从缓冲区起始空间读取
        if (rlen < len)                                    
            memcpy(data + rlen, mpDataBuffer, len - rlen);

        // ringbuf->rpos += len; // 允许多次读
        return len;
    }

    unsigned int CCJGW_RingBuffer::DrainRingBuffer(unsigned int len)
    {
        _ASSERT(mpRingBuffer_t);

        if (len > mpRingBuffer_t->wpos - mpRingBuffer_t->rpos)
            // 最大为缓冲区数据长度
            len = mpRingBuffer_t->wpos - mpRingBuffer_t->rpos; 

        mpRingBuffer_t->rpos += len;
        return len;
    }

    unsigned int CCJGW_RingBuffer::SearchRingBuffer(unsigned char *sep, unsigned int seplen)
    {
        _ASSERT(mpRingBuffer_t && sep);

        unsigned int len = mpRingBuffer_t->wpos - mpRingBuffer_t->rpos; // 缓冲区数据长度
        if (len < seplen)
            return 0;

        for (unsigned int i = 0; i <= len - seplen; i++)
        {
            unsigned int idx = (mpRingBuffer_t->rpos + i) & (mpRingBuffer_t->size - 1); // 当前比较索引

            if (idx + seplen <= mpRingBuffer_t->size) // “比较区间”连续，只需要一次比较
            {
                if (memcmp(mpDataBuffer + idx, sep, seplen) == 0)
                    return i + seplen;
                continue;
            }

            unsigned int len1 = mpRingBuffer_t->size - idx; // “比较区间”不连续，需要两次比较
            if (memcmp(mpDataBuffer + idx, sep, len1) == 0 &&
                memcmp(mpDataBuffer, sep + len1, seplen - len1) == 0)
                return i + seplen;
        }

        return 0;
    }
}

