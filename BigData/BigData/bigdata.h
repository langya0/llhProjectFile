#include"afxstd.h"


#define UN_INIT 0xcccccccccccccccc
#define MAX_INT64 0x7fffffffffffffff
#define MIN_INT64 0x8000000000000000

const long long cMAX_INT64 = MAX_INT64;
const long long cMIN_INT64 = MIN_INT64;

typedef long long INT64;

class BigData
{
public:
	BigData(const char *str)
		:m_llData(UN_INIT)
		, m_strData("")
	{
		//尝试用字符串去初始化
		TryInitByStr(str);
	}
	
	BigData(INT64 data = UN_INIT)
		:m_llData(data)
		,m_strData("")
	{}
	BigData(const BigData& data)
		:m_llData(data.m_llData)
		, m_strData(data.m_strData)
	{}
	BigData& operator=(const BigData&data)
	{
		if (this != &data)
		{
			m_llData = data.m_llData;
			m_strData = data.m_strData;
		}
		return *this;
	}

	BigData operator+(const BigData&);
	BigData operator-(const BigData&);
	BigData operator*(const BigData&);
	BigData operator/(const BigData&);
	BigData operator%(const BigData&);
	friend ostream& operator<<(ostream& _out, const BigData&data);
	friend istream& operator>>(istream& _in, BigData&data);

	bool operator<(const BigData&data);
	bool operator>=(const BigData&);
	bool operator>(const BigData&);
	bool operator==(const BigData&);
	string INT64ToString(INT64 llData);
	bool IsINT64Owerflow()const;
protected:
	void TryInitByStr(const char* str);

	bool Big(const BigData&data);
	BigData Add(const BigData & data);
	BigData Sub(const BigData & data);
	BigData Mul(const BigData & data);
	BigData Div(const BigData & data);
	
	bool IsLeftStrBig(char *pLeft, size_t LSize, char *pRight, size_t RSize);
	char SubLoop(char *pLeft, size_t LSize, char *pRight, size_t RSize);
	
protected:
	long long m_llData;
	string m_strData;
};

//初始化：判断是否溢出，确定初始化位置。
//
