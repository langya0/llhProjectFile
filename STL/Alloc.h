#pragma once

#include "Config.h"
#include "Trace.h"

#include "Threads.h"

#ifdef __STL_THREADS
#define __NODE_ALLOCATOR_THREADS true  //用于二级空间配置器翻非类型模板参数

#define __NODE_ALLOCATOR_LOCK \
        { if (threads) _S_node_allocator_lock._M_acquire_lock(); }
#define __NODE_ALLOCATOR_UNLOCK \
        { if (threads) _S_node_allocator_lock._M_release_lock(); }
#else
//  Thread-unsafe
#   define __NODE_ALLOCATOR_LOCK
#   define __NODE_ALLOCATOR_UNLOCK
#   define __NODE_ALLOCATOR_THREADS false
#endif

__STLBEGIN
#include <memory.h>
#include <stdlib.h>

#define __THROW_BAD_ALLOC throw 

class __MallocAllocTemplate
{
private:
	static void *OomMalloc(size_t);
	static void *OomRealloc(void *, size_t);
	static void(*__malloc_alloc_oom_handler)();
public:
	static void *Allocate(size_t n)
	{
		__TRACE("__MallocAllocTemplate to get  n = %u\n",n);
		void *result = malloc(n);
		if (0 == result)
		{
			result = OomMalloc(n);
		}
		return result;
	}
	static void *Reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (0 == result)
		{
			result = OomRealloc(p, new_sz);
			return result;
		}
	}

	static void Deallocate(void *p,size_t n)
	{
		__TRACE("一级空间配置器释放 p= %p n = %u\n",p,n);
		if(p)
			free(p);
	}

	//
	static void(*set_malloc_handler(void(*f)()))()
	{
		__TRACE("一级空间配置器,set Handler f = %p\n",f);
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}

};

void *__MallocAllocTemplate::OomMalloc(size_t n)
{
	__TRACE("一级空间配置器,不足进入Oo中n = %u\n",n);
	void(*my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
		{
			__THROW_BAD_ALLOC;
		}
		(*__malloc_alloc_oom_handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

void* __MallocAllocTemplate::OomRealloc(void* p, size_t n)
{
	void(*my_malloc_handler)();
	void* result;

	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
		{
			__THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}
void(*__MallocAllocTemplate::__malloc_alloc_oom_handler)() = 0;


typedef __MallocAllocTemplate MallocAlloc;	//////这里放在#ifdef前边是因为二级空间配置器中还需要调用一级空间配置器
/////////////////////////////////////////根据是否配置__USE_ALLOC选择使用一级还是二级空间配置器

#ifdef __USE_ALLOC
typedef MallocAlloc Alloc;
#else //not define   __USE_ALLOC


# ifdef __STL_THREADS
    static _STL_mutex_lock _S_node_allocator_lock;
# endif

template <bool threads, int inst>
class __DefaultAllocTemplate 
{
protected:

	enum {_ALIGN = 8};
	enum {_MAX_BYTES = 128};
	enum {_NFREELISTS = 16}; // _MAX_BYTES/_ALIGN

	static size_t  RoundUp(size_t bytes) 
	{
	 return (((bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); 
	}


    // It would be nice to use _STL_auto_lock here.  But we
    // don't need the NULL check.  And we do need a test whether
    // threads have actually been started.
    class _Lock;
    friend class _Lock;
    class _Lock {
        public:
            _Lock() 
            {
            	__TRACE("锁保护\n");
             __NODE_ALLOCATOR_LOCK;
             }
            ~_Lock()
            {
            	__TRACE("锁撤销\n");
             __NODE_ALLOCATOR_UNLOCK; 
         	}
    };

protected:
	union _Obj {
		// 节点链接指针
	union _Obj* _freeListLink;
	//客户端数据
    char _ClientData[1];             
	};

	//桶结构,保存链表
	static _Obj* _freeList[_NFREELISTS]; 

	//获取具体大小元素在表中的下标
	static  size_t _FreeListIndex(size_t __bytes) 
	{
	    return (((__bytes) + (size_t)_ALIGN-1)/(size_t)_ALIGN - 1);
	}

	static char* _start_free;
	static char* _end_free;
	static size_t _heap_size;
public:
	static void* Allocate(size_t n)
	{
		void * ret = 0;
		__TRACE("二级空间配置器申请n = %u\n",n);
		if(n>_MAX_BYTES)
			ret = MallocAlloc::Allocate(n);

		_Obj* volatile * __my_free_list = _freeList + _FreeListIndex(n);

		//利用RAII（资源获取即初始化原则）进行封装，保证 即使内部抛出异常，依旧执行解锁操作
#ifdef __STL_THREADS
      	_Lock __lock_instance;
#endif
		_Obj* __result = *__my_free_list;
	
		if (__result == 0)
			ret = _Refill(RoundUp(n));
		else
		{
			*__my_free_list = __result -> _freeListLink;
			ret = __result;
		}
	    return ret;
    }

	static void Deallocate(void* p, size_t n)
	{
		if(!p)
		{
			return;
		}
		__TRACE("二级空间配置器删除p = %p,n = %d\n",p,n);
		if (n > (size_t) _MAX_BYTES)
			MallocAlloc::Deallocate(p, n);
		else
		{
			_Obj* volatile*  __my_free_list = _freeList + _FreeListIndex(n);
			_Obj* q = (_Obj*)p;
		
#ifdef __STL_THREADS
			//进行资源归还自由链表时的锁操作。
      		_Lock __lock_instance;
#endif
			q -> _freeListLink = *__my_free_list;
			*__my_free_list = q;
		}
	}

	static void *Reallocate(void* p,size_t __old_sz,size_t __new_sz)
	{
		__TRACE("二级空间配置器重新申请p = %p,new_sz = %d\n",p,__new_sz);
	    void* __result;
	    size_t __copy_sz;

	    if (__old_sz > (size_t)_MAX_BYTES && __new_sz > (size_t)_MAX_BYTES)
	    {
	        return(realloc(p, __new_sz));
	    }

	    if (RoundUp(__old_sz) == RoundUp(__new_sz))
	    	return(p);

	    __result = Allocate(__new_sz);
	    
	    __copy_sz = __new_sz > __old_sz? __old_sz : __new_sz;
	    memcpy(__result, p, __copy_sz);
	    Deallocate(p, __old_sz);
	    return(__result);
	}	
protected:
	static void *_Refill(size_t n)
	{
		__TRACE("二级空间配置器自由链表填充n = %u\n",n);

		size_t nobjs = 20;
		void * ret;
		char * chunks = _ChunkAlloc(n,nobjs);

		if(nobjs == 1)
		{
			return chunks;
		}
		_Obj* volatile * __my_free_list = _freeList+_FreeListIndex(n);
		ret = chunks;

		for(size_t i = 1;i<nobjs;i++)
		{
			((_Obj*)(chunks+n*i))->_freeListLink = *__my_free_list;
			*__my_free_list = (_Obj*)(chunks+n*i); 
		}
		return ret; 
	}

	/////////////这里的nobjs使用&，，内部需要复用逻辑，可能改变之
	static char * _ChunkAlloc(size_t n,size_t &nobjs)
	{
		size_t totalBytes = n*nobjs;
		size_t bytesLeft = _end_free - _start_free;

		if(bytesLeft>=totalBytes)
		{
			__TRACE("二级空间配置器内存池填充n = %u,nobjs = %d\n",n,nobjs);
			_start_free += n*nobjs;
			return _start_free;
		}
		else if(bytesLeft>=n)
		{
			nobjs = (_end_free- _start_free)/n;
			__TRACE("二级空间配置器内存池填充n = %u,nobjs = %d\n",n,nobjs);
			_start_free +=n*nobjs;
			return _start_free;
		}

		//bytesLeft [0,1)
		_Obj*  volatile * __my_free_list = _freeList + _FreeListIndex(bytesLeft);
		if(_start_free)
		{
			if(bytesLeft>0)
			{
				((_Obj*)_start_free)->_freeListLink=*__my_free_list;
				*__my_free_list = (_Obj*)_start_free;
			}
		}
		size_t bytesToGet = nobjs*n*2+(_heap_size>>4); 

		__TRACE("bytesToGet = %u\n",bytesToGet);
		//malloc
		_start_free = (char*)malloc(bytesToGet);
		char *_start_free3 = (char*)malloc(100);
		__TRACE("_start_free3 = %p\n",_start_free3);

		if(!_start_free)
		{
			__TRACE("二级空间配置器malloc失败，在后续链表查找n = %d\n",n);
			for(size_t i = n + _ALIGN;i < _MAX_BYTES;i+=_ALIGN)
			{
				_Obj* volatile * cur = _freeList+_FreeListIndex(i); 
				if(*cur)
				{
					*cur = (*cur)->_freeListLink;
					_start_free = (char*)*cur;
					_end_free = _start_free + i;

					return _ChunkAlloc(n,nobjs);
				}
			}

			__TRACE("二级空间配置器：后续链表查找失败，转接一级配置，借用handler机制终止程序或者得到空间n = %d\n",n);
			_start_free = (char*)MallocAlloc::Allocate(n);
			return _ChunkAlloc(n,nobjs);
		}
		else
		{
			_end_free = _start_free + bytesToGet;
			_heap_size += bytesToGet;
			return _ChunkAlloc(n,nobjs);
		}

	}
};

template <bool __threads, int __inst>
char* __DefaultAllocTemplate<__threads, __inst>::_start_free= 0;

template <bool __threads, int __inst>
char* __DefaultAllocTemplate<__threads, __inst>::_end_free = 0;

template <bool __threads, int __inst>
size_t __DefaultAllocTemplate<__threads, __inst>::_heap_size = 0;


	// static _Obj* _freeList[_NFREELISTS];
template <bool threads, int inst>
typename __DefaultAllocTemplate<threads,inst>::_Obj* 
__DefaultAllocTemplate<threads,inst>::_freeList[__DefaultAllocTemplate<threads,inst>::_NFREELISTS]
 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

typedef __DefaultAllocTemplate<__NODE_ALLOCATOR_THREADS,0>  Alloc;

#endif

__STLEND

void handler()
{
	__TRACE("here in  handler!\n");
}
void testAlloc()   
{
//	stl::Alloc::set_malloc_handler(handler);
	void *arr[21] = {0};
	__TRACE("Clint to Get size = %u\n",5);

	// for(size_t i =0;i < 21;++i)
	// 	arr[i] = stl::Alloc::Allocate(5);
	// for(size_t i =0;i < 21;++i)
	// 	stl::Alloc::Deallocate(arr[i],5);
	arr[1] = stl::Alloc::Allocate(8);
	arr[2] = stl::Alloc::Allocate(72);

	arr[3] = stl::Alloc::Allocate(80);
}