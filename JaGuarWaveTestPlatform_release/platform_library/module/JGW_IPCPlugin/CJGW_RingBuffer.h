#pragma once
#include <string>
// 环形缓冲区分隔符最大长度为10
#define RING_BUFFER_SEQARATOR_LENGTH 10
namespace JGW
{
    typedef struct ringbuffer_t
    {
        /// @brief 缓存大小，值为 2 的整数次幂，便于“求余运算”，以及“溢出后对齐”
        volatile unsigned int size;
        /// @brief 累加后的写位置，数据实际在 (wpos % size) 索引位置写入
        volatile unsigned int wpos;
        /// @brief 累加后的读位置，数据实际在 (rpos % size) 索引位置读出
        volatile unsigned int rpos;
    } ringbuffer_t;

    class CCJGW_RingBuffer
    {
    public:
        CCJGW_RingBuffer(void);
        ~CCJGW_RingBuffer(void);
    public:
        /// @brief 创建环形缓冲区
        /// @param nBufferLen 环形缓冲区BUFFER大小
        bool CreateRingBuffer(size_t nBufferLen);
        /// @brief 创建环形缓冲区
        /// @param pRingBuffer_t 环形缓冲区结构体
        /// @param pBuffer 环形缓冲区BUFFER
        /// @return true : 成功 false : 失败
        bool CreateRingBuffer(ringbuffer_t* pRingBuffer_t,void* pBuffer);
        /// @brief 创建环形缓冲区
        /// @param pRingInfoBuf 环形缓冲区结构体
        /// @param pBuffer 环形缓冲区BUFFER
        /// @param nBufferLen 环形缓冲区BUFFER大小
        /// @return true : 成功 false : 失败
        bool CreateRingBuffer(ringbuffer_t* pRingBuffer_t,void* pBuffer,size_t nBufferLen);
        /**
        * @brief 向 buf 缓冲区中写入数据
        * @details 如果缓冲区剩余空间不足，直接返回失败，不会写入任何数据。
        *
        * @param[in] buf  缓冲区指针
        * @param[in] data 待写入数据的指针
        * @param[in] len  待写入数据的长度
        *
        * @return 0 为成功；
        *         -1 为缓冲区剩余空间不足（len > 剩余空间）
        */
        int WriteRingBuffer(unsigned char *data, unsigned int len);

        /**
        * @brief 从 buf 缓冲区中读取数据
        * @details 连续调用 buffer_read 会读取同一地址数据；
        *          如果读取后面数据，需要通过 buffer_drain() 向前移动读指针。
        *
        * @param[in] buf   缓冲区指针
        * @param[out] data 待存储数据的空间指针
        * @param[in] size  data 指向空间的大小
        *
        * @return 实际读取数据的长度
        */
        unsigned int ReadRingBuffer(unsigned char *data, unsigned int size);

        /**
        * @brief 向前移动 buf 缓冲区中读指针的位置
        * @details 在调用 buffer_read() 后，读指针仍然在原来的位置不变，
        *          可以根据数据的实际使用长度，通过 buffer_drain() 向前移动读指针，
        *          以便下一次调用 buffer_read() 时，从新的地址读取数据。
        *
        * @param[in] buf 缓冲区指针
        * @param[in] len 读指针向前移动长度，即需要舍弃的数据长度
        *
        * @return 读指针实际向前移动的长度（不会超过缓冲区中数据的长度）
        */
        unsigned int DrainRingBuffer(unsigned int len);
        /**
        * @brief 在 buf 缓冲区的数据中，检索指定分隔符。
        * @details 如果数据中包含多个分隔符，通过多次调用 buffer_search() 和 buffer_read()，
        *          直到 buffer_search() 返回 0，可以取出所有完整的数据包。
        *
        * @param[in] buf    缓冲区指针
        * @param[in] sep    分隔符指针
        * @param[in] seplen 分隔符长度
        *
        * @return 第 1 次出现分隔符的数据长度（包含分隔符）；
        *         如果没有找到分隔符，返回 0
        */
        unsigned int SearchRingBuffer(unsigned char *sep, unsigned int seplen);
    private:
        bool mbIsSelfCreateBuffer;
        //! 环形缓冲区BUFFER
        unsigned char* mpDataBuffer;
        //! 环形缓冲区结构体，保存读取写入信息
        ringbuffer_t* mpRingBuffer_t;
    };
}


