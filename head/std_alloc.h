#pragma once
//#define __USE_MALLOC

#if 0
# include <new>
#define __THROW_BAD_ALLOC throw std::bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include<iostream>
#define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1);
#endif

//一级分配器
template<int inst>
class __malloc_alloc_template {
private:
    static void* oom_malloc(size_t);
    static void* oom_realloc(void*, size_t);
    static void (*__malloc_alloc_oom_handler)();
public:
    //直接使用malloc分配内存，如果失败则调用oom_malloc一直申请。
    static void* allocate(size_t n)
    {
        void* result = malloc(n);
        if (0 == result)
            result = oom_malloc(n);
        return result;
    }
    //直接使用free释放内存
    static void deallocate(void* p, size_t)
    {
        free(p);
    }
    //使用realloc扩容内存，如果失败则调用oom_realloc一直扩。
    static void* reallocate(void* p, size_t old_sz, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if (0 == result)
            result = oom_realloc(p, new_sz);
        return result;
    }

    //函数名：set_malloc_handler  参数：void (*)() 返回类型： void(*)()
    //设置__malloc_alloc_oom_handler，在使用oom_malloc、oom_realloc之前首先先调用此函数
    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }
};

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
//二级分配器
template<bool threads, int inst>
class __default_alloc_template {
private:
    static size_t ROUND_UP(size_t bytes) {
        return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
    }
    //free_list节点结构
    union obj {
        union obj* free_list_link;
        char client_data[1];
    };
    //16个free_list
    static obj* volatile free_list[__NFREELISTS];

    //根据区块大小，决定使用n号free_list;
    static size_t FREELISTS_INDEX(size_t bytes) {
        return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
    }

    //返回一个大小为n的对象，并可能加入大小为n的其他区块到free list
    static void* refill(size_t n);

    //配置一块大的空间，可容纳nobjs个大小为size的区块
    //如果配置nobjs个大小为size的区块有所不便， nobjs可能会降低
    static char* chunk_alloc(size_t size, int& nobjs);

    //内存池起始位置
    static char* start_free;
    //内存池终止位置
    static char* end_free;
    static size_t heap_size;

public:
    static void* allocate(size_t n) {
        obj* volatile* my_free_list;
        obj* result;
        //如果大于128调用一级分配器
        if (n > (size_t)__MAX_BYTES) {
            return (__malloc_alloc_template<0>::allocate(n));
        }
        my_free_list = free_list + FREELISTS_INDEX(n);
        result = *my_free_list;
        if (result == 0) {
            void* r = refill(ROUND_UP(n));
            return r;
        }
        //my_list的适当位置向前推进
        *my_free_list = result->free_list_link;
        return result;
    }
    static void deallocate(void* p, size_t n) {
        obj* q = (obj*)p;
        obj* volatile* my_free_list;
        //如果大于128调用一级分配器
        if (n > (size_t)__MAX_BYTES)
        {
            __malloc_alloc_template<0>::deallocate(p, n);
            return;
        }
        //吧刚释放的内存放到free_list适当位置的前面
        my_free_list = free_list + FREELISTS_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    //并未实现，只是提供一个接口
    static void* reallocate(void* p, size_t old_sz, size_t new_sz);
};


//如果定义了 __USE_MALLOC，则alloc定义为一级分配器，否则为二级
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<false, 0> alloc;
#endif // __USE_MALLOC

//初始化
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (*my_malloc_handler)();
    void* result;
    for (;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler)
            __THROW_BAD_ALLOC
            (*my_malloc_handler)();
        result = malloc(n);
        if (result)
            return result;
    }
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
    void (*my_malloc_handler)();
    void* result;
    for (;;)
    {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler)
            __THROW_BAD_ALLOC
            (*my_malloc_handler)();
        result = realloc(p, n);
        if (result)
            return result;
    }
}



//static data member初始值
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


//chunk_alloc从内存池取空间给freelist用
template<bool threads, int inst>
char*
__default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
    char* result;
    size_t total_bytes = size * nobjs;
    //内存池剩余空间
    size_t bytes_left = end_free - start_free;
    if (bytes_left >= total_bytes)
    {
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else if (bytes_left >= size)
    {
        //内存池不够nobjs个size，但是够一个以上size
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else
    {
        //内存池连一个size都给不出来
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        if (bytes_left > 0)
        {
            //挖掘剩余价值
            obj* volatile* my_free_list = free_list + FREELISTS_INDEX(bytes_left);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        }

        //补充内存池
        start_free = (char*)malloc(bytes_to_get);
        if (0 == start_free)
        {
            //malloc失败
            int i;
            obj* volatile* my_free_list, * p;//p的类型是obj *
            //从小内存中征收
            for (i = size; i <= __MAX_BYTES; i += __ALIGN)
            {
                my_free_list = free_list + FREELISTS_INDEX(i);
                p = *my_free_list;
                if (0 == p)
                {
                    *my_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + i;
                    //为了修正nobjs
                    return (chunk_alloc(size, nobjs));
                }
            }
            end_free = 0;
            //山穷水尽了，看看一阶分配器能不能用
            start_free = (char*)__malloc_alloc_template<0>::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        //为了修正nobjs
        return chunk_alloc(size, nobjs);
    }
}


template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    obj* volatile* my_free_list;
    obj* result;
    obj* current_obj, * next_obj;
    int i;
    //如果只分配了一块，则直接返回，不用加到free list表中
    if (1 == nobjs)
        return chunk;
    my_free_list = free_list + FREELISTS_INDEX(n);
    result = (obj*)chunk;
    *my_free_list = next_obj = (obj*)(chunk + n);
    for (i = 1;; i++)
    {
        current_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = 0;
            break;
        }
        else
            current_obj->free_list_link = next_obj;
    }
    return (result);
}




//多一层包装，在vector<int, Alloc = alloc>的时候会使用alloc构建一个simple_alloc。多一层封装
template<class T, class Alloc>
class simple_alloc {
public:
    static T* allocate(size_t n)
    {
        return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
    }
    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T* p, size_t n)
    {
        if (0 != n)
            Alloc::deallocate(p, n * sizeof(T));
    }
    static void deallocate(T* p)
    {
        Alloc::deallocate(p, sizeof(T));
    }
};
