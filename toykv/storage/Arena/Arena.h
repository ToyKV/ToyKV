#ifndef ARENA_ARENA_H
#define ARENA_ARENA_H



class Arena
{
public:
    //构造函数和析构函数
    Arena();
    ~Arena();

    //1.分配指定大小的内存，并返回分配内存的首地址
    char* Allocate(size_t bytes);

    //2.分配指定大小并且对齐的内存
    char* AllocateAligned(size_t bytes);


    //3.统计使用了多少内存
    size_t memoryUsage() const;




private:
    //4.分配内存,根据情况决定是否使用新的block
    char* AllocateFallback(size_t bytes);

    //5.分配一个新的block
    char* AllocateNewBlock(size_t block_bytes);

    //指向一个block中未被使用的内存首地址
    char* alloc_ptr_;


    //表示一个block还剩余多少空间
    size_t alloc_bytes_remaining_;


    //内存池数组
    std::vector<char*> blocks_;


    //统计内存使用，原子变量(避免并发访问时的线程安全问题)
    port::AtomicPointer memory_usage_;


    //将拷贝构造和赋值构造设置为私有
    Arena(const Arena&);
    void operator=(const Arena&);

};