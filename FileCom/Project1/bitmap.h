#pragma once
#include"stdafx.h"

#define TYPE int
const int TYPE_SIZE = 32;
class BitMap
{
public:
	BitMap()
		:_size(0)
	{
		_setv.resize(1);
		_setv.assign(1, 0);
	}
	BitMap(int x)
		:_size(x)
	{
		int size = (x >> 5) + 1;
		_setv.resize(size);
		_setv.assign(size, 0);
	}
	//set pos 1
	bool set(int pos)
	{
		assert(pos <= _size&&pos >= 1);

		int index = pos % TYPE_SIZE;
		if (_setv[(pos - 1) / TYPE_SIZE] & (0x1 << index - 1))
		{
			return false;
		}

		_setv[(pos - 1) / TYPE_SIZE] |= (0x1 << index - 1);
		return true;
	}
	//set pos 0
	bool reset(int pos)
	{
		assert(pos <= _size&&pos >= 1);

		int index = pos % TYPE_SIZE;
		if (!(_setv[(pos - 1) / TYPE_SIZE] & (0x1 << index - 1)))
		{
			return false;
		}

		_setv[(pos - 1) / TYPE_SIZE] &= (~(0x1 << index - 1));
		return true;
	}
	bool operator==(const BitMap&s)const
	{
		if (_size != s._size)
			return false;
		for (int i = 0; i < _setv.size(); ++i)
		{
			if (_setv[i] != s._setv[i])
				return false;
		}
		return true;
	}
	size_t size()const
	{
		return _size;
	}
	void PrintHex()
	{
		int index = _setv.size();
		for (int i = index - 1; i >= 0; --i)
		{
			printf("%0x", _setv[i]);
		}
		cout << endl;
	}
	void clear()
	{
		_setv.assign(_setv.size(), 0);
	}
	void PrintByte()
	{
		for (int i = _size; i > 0; --i)
		{
			if (i % 8 == 0)
				cout << " ";

			cout << ((_setv[(i - 1) / TYPE_SIZE] & (0x1 << (i % TYPE_SIZE) - 1)) != 0);
		}
		cout << endl;
	}

	bool test(size_t pos)const 
	{
		assert(pos <= _size&&pos >= 1);
		return (_setv[(pos - 1) / TYPE_SIZE] & (0x1 << ((pos % TYPE_SIZE) - 1))) != 0;
	}

	bool revers()
	{
		size_t pos = _size;
		int i = 1;
		while(i<pos)
		{
			
			bool bTest = test(i);
			bool eTest = test(pos);
			if (!bTest)
				reset(pos);
			else
				set(pos);
			if (!eTest)
				reset(i);
			else
				set(i);
			i++;
			pos--;
		}
		return true;
	}
	bool append(int I)
	{
		int a[1] = { 0 };
		if (I == 1)
		{
			a[0] = 1;
			append(a, 1);
		}
		if (I == 0)
		{
			append(a, 1);
		}
		return true;
	}
	bool append(int *arr, size_t n)
	{
		//①确定增加后vector长度，空间处理
		size_t end = _setv.size();
		if ((_size + n)>TYPE_SIZE*end)
		{
			_setv.resize((_size + n + TYPE_SIZE-1) / TYPE_SIZE);
		}
		for (int i = end; i < _setv.size(); ++i)
		{
			_setv[i] = 0;
		}
		//②依次设置
		for (int i = 0; i < n; ++i)
		{
			if (arr[i] == 1)
			{
				set(++_size);
			}
			else
			{
				reset(++_size);
			}
		}	
		return true;
	}
	//+=
	bool Merge(const BitMap& bm)
	{
		//①确定增加后vector长度，空间处理
		size_t end = _setv.size();
		if ((_size + bm._size)>TYPE_SIZE * end)
		{
			_setv.resize((_size + bm._size + TYPE_SIZE-1) / TYPE_SIZE);
		}
		for (int i = end; i < _setv.size(); ++i)
			_setv[i] = 0;

		//②依次设置
		for (int i = bm._size; i >= 1; --i)
		//for (int i = 1; i <= bm._size; ++i)
		{
			if (bm.test(i))
				set(++_size);
			else
				++_size;//reset(++_size);
		}
		return true;
	}
public:
	vector<TYPE> _setv;
public:
	size_t _size;
};