#pragma once

#include "Config.h"
__STLBEGIN
#include <memory.h>
#include <stdlib.h>

#define __THROW_BAD_ALLOC throw std::bad_alloc()

class __MallocAllocTemplate
{
private:
	static void *OomMalloc(size_t);
	static void *OomRealloc(void *, size_t);
	static void(*__malloc_alloc_oom_handler)();
public:
	static void *Allocate(size_t n)
	{
		___TRACE("__MallocAllocTemplate to get  n = %u\n",n);
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
		___TRACE("一级空间配置器释放 p= %p n = %u\n",p,n);
		free(p);
	}

	//
	static void(*set_malloc_handler(void(*f)()))()
	{
		___TRACE("一级空间配置器,set Handler f = %p\n",f);
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}

};

void *__MallocAllocTemplate::OomMalloc(size_t n)
{
	___TRACE("一级空间配置器,不足进入Oo中n = %u\n",n);
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

typedef __MallocAllocTemplate MallocAlloc;///////////////////////

#ifdef __USE_ALLOC
typedef MallocAlloc Alloc;
#else //not define   __USE_ALLOC

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
		___TRACE("二级空间配置器申请n = %u\n",n);
		if(n>_MAX_BYTES)
			ret = MallocAlloc::Allocate(n);

		_Obj* volatile * __my_free_list = _freeList + _FreeListIndex(n);

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
		___TRACE("二级空间配置器删除p = %p,n = %d\n",p,n);
		if (n > (size_t) _MAX_BYTES)
			MallocAlloc::Deallocate(p, n);
		else
		{
			_Obj* volatile*  __my_free_list = _freeList + _FreeListIndex(n);
			_Obj* q = (_Obj*)p;
			q -> _freeListLink = *__my_free_list;
		*__my_free_list = q;
		}
	}

	static void *Reallocate(void* p,size_t __old_sz,size_t __new_sz)
	{
		___TRACE("二级空间配置器重新申请p = %p,new_sz = %d\n",p,__new_sz);
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
		___TRACE("二级空间配置器自由链表填充n = %u\n",n);

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

	static char * _ChunkAlloc(size_t n,size_t nobjs)
	{
		size_t totalBytes = n*nobjs;
		size_t bytesLeft = _end_free - _start_free;

		if(bytesLeft>=totalBytes)
		{
			___TRACE("二级空间配置器内存池填充n = %u,nobjs = %d\n",n,nobjs);
			_start_free += n*nobjs;
			return _start_free;
		}
		else if(bytesLeft>=n)
		{
			nobjs = (_end_free- _start_free)/n;
			___TRACE("二级空间配置器内存池填充n = %u,nobjs = %d\n",n,nobjs);
			_start_free +=n*nobjs;
			return _start_free;
		}

		//bytesLeft [0,1)
		_Obj*  volatile * __my_free_list = _freeList + _FreeListIndex(bytesLeft);
		if(_start_free)
		{
			___TRACE("二级空间配置器剩余bytesLeft = %u\n",bytesLeft);
			((_Obj*)_start_free)->_freeListLink=*__my_free_list;
			*__my_free_list = (_Obj*)_start_free;
		}

		size_t bytesToGet = nobjs*n*2+(_heap_size>>4); 

		//malloc
		_start_free = (char*)malloc(bytesToGet);

		if(!_start_free)
		{
			___TRACE("二级空间配置器malloc失败，在后续链表查找n = %d\n",n);
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



typedef __DefaultAllocTemplate<0,0>  Alloc;
#endif

__STLEND

void handler()
{
	cout << "here in  handler!\n"<<endl;
}
void test()
{
//	stl::Alloc::set_malloc_handler(handler);
	void *arr[21] = {0};
	___TRACE("Clint to Get size = %u\n",5);

	// for(size_t i =0;i < 21;++i)
	// 	arr[i] = stl::Alloc::Allocate(5);
	// for(size_t i =0;i < 21;++i)
	// 	stl::Alloc::Deallocate(arr[i],5);
	arr[1] = stl::Alloc::Allocate(8);
	arr[2] = stl::Alloc::Allocate(72);

	arr[3] = stl::Alloc::Allocate(80);
}