#include "Arena.h"

//1.返回分配好的内存段的指针地址
char* Arena::Allocate(size_t bytes)
{
    //断言:判断开辟空间大小字节数的合法性
    assert(bytes > 0);

    // 判断是否有可被复用的block
    if (bytes <= alloc_bytes_remaining_)
    {
        char* result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;

        return result;
    }

    return AllocateFallback(bytes);
}



//2.返回分配指定字节大小的内存段地址,还要满足内存对齐等规则
char* Arena::AllocateAligned(size_t bytes)
{
    //获取当前系统指针大小
    const int align;

    if(sizeof(void*) > 8)
        align=sizeof(void*);
    else
        align=8;

    //指针大小必须是2的整数次幂，2的整数次幂的二进制表示中
    //有且只有1位是1
    assert((align & (align-1)) == 0);

    //判断bytes是不是align 的整数倍，由于align是2的
    //整数次幂，所以对align的取模运算可以转化为
    //对（align - 1）进行按位与
    size_t current_mod = (alloc_ptr_) & (align-1);


    //为了对齐内存需要新增的大小
    size_t slop;
    if(current_mod == 0)
        slop = 0;
    else
        slop = align - current_mod;


    //needed 表示分配对齐的内存所需的大小
    size_t needed = bytes + slop;

    // 判断是否有可被复用的block
    char* result;
    if (needed <= alloc_bytes_remaining_)
    {
        result = alloc_ptr_ + slop;
        alloc_ptr_ += needed;
        alloc_bytes_remaining_ -= needed;
    }

    else
    {
        result = AllocateFallback(bytes);
    }


    assert((result) & (align-1)) == 0);

    return result;
}


//3.返回内存池分配的总体内存空间大小
size_t memoryUsage() const
{
    return memory_usage_ + blocks_.capacity() * sizeof(char*);
}
9



//4.分配内存，根据情况决定是否使用新的block
//函数的作用是分配一个新的block。不同的是，如果bytes小于blocksize的四分之一，
//则此新分配的block 可以被继续复用。否则的话直接分配新的block,Arena中可被复用的block保持不变。
char* Arena::AllocateFallback(size_t bytes)
{
    //分配的bytes较小，此新block可以被复用
    if (bytes > blockSize/4)
    {
        char* result = AllocateNewBlock(bytes);

        return result;
    }

    else
    {
        //此新分配的block可以被复用，重新设置
        alloc_ptr_ = AllocateNewBlock(blockSize);
        alloc_bytes_remaining_ = blockSize;
        char* result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;

        return result;
    }

}


//5.直接使用new分配符分配指定大小的内存，并且更新memory_usage_
//直接使用new分配符分配指定大小的内存，并且更新memory_usage_
char* Arena::AllocateNewBlock(size_t block_bytes)
{
    //分配空间
    char* result = new char[block_bytes];

    //新的block 加入内存池
    blocks_.push_back(result);

    //更新memory_usage_
    memory_usage = memoryUsage() + block_bytes*sizeof(char*);

    return result;
}



//6.析构函数:逐个释放内存池中的block
Arena::~Arena()
{
    for (size_t i = 0; i < blocks_.size(); i++)
    {
        delete[] blocks_[i];
    }
}
