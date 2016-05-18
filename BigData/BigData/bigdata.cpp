#include"bigdata.h"

string BigData::INT64ToString(INT64 lldata)
{
	//1确定符号位
	//2按位存储

	INT64 tmp = lldata;
	string retData("+");
	if (lldata < 0)
	{
		retData[0] = '-';
		tmp *= -1;
	}
	while (tmp)
	{
		int cTemp = (tmp) % 10+'0';
		retData.append(1,cTemp);
		tmp /= 10;		
	}

//	m_strData = retData;
	long Size = retData.size()-1;
	for (long begin = 1; begin<= Size; ++begin, --Size)
	{
		::swap(retData[begin], retData[Size]);
	}
	
	return retData;
}

ostream& operator<<(ostream& _out,const BigData&data)
{
	if (string("") != data.m_strData)
	{
		_out << data.m_strData;
	}
	else
	{
		_out << data.m_llData;
	}
	return _out;
}

istream& operator>>(istream& _in, BigData&data)
{
	//输入字符串，判断是否
	return _in;
}

BigData operator+(const BigData&)
{
	//
	return BigData();
}

//atoi 特点：00123abc  --》123
//			 00 123abc --》0
//so：1消除前置空格，
//2 消除前置0，
//3遇到非数字即结束。
void BigData::TryInitByStr(const char *str)
{

	//合法化，然后判断是否溢出
	string retData;
	retData.append(1, '+');

	//确定符号位以及第一个有效位
	int index = 0;
	int FlagOfData = 1;

	//去除前置空格
	while (*(str + index) == ' ')
		index++;
	if (str[index] == '-')
	{
		retData[0] = '-';
		index += 1;
		FlagOfData = -1;
	}
	else if (str[index] == '+')
		index += 1;

	//去除前置0
	while (*(str + index) == '0')
	{
		index++;
		continue;
	}

	INT64 m_TempData = 0;
	//得到第一个非0位置。同时直接实现初始化m_llData
	while (*(str + index) >= '0'&&*(str + index) <= '9')
	{
		retData.append(1, *(str + index));
		m_TempData = m_TempData * 10 + (*(str + index) - '0');
		index++;
	}

	//将结果保存到m_llData;
	m_TempData *= FlagOfData;
	m_llData = m_TempData;

	//得到合法化的  负数字符串模式数据
	//比较判断是否溢出
	if (retData[0] == '-'
		&&
		(retData.size() == 20
		&& (strcmp(retData.c_str(), "-9223372036854775808") > 0)
		|| retData.size() > 20   //合法负数结果长度过大
		)
		)
	{
		m_strData = retData;
		return;
	}
	if (retData[0] == '+'
		&&
		(retData.size() == 20
		&& (strcmp(retData.c_str(), "+9223372036854775807") > 0)
		||retData.size()>20	
		)
		)
	{
		m_strData = retData;
		return;
	}
	m_strData = "";
}

BigData BigData::operator+(const BigData&data)
{
	//两个操作数都没有溢出，结果也没有溢出
	if (data.m_strData.size() == 0 && m_strData.size() == 0
		&& m_llData + data.m_llData <= LLONG_MAX
		//fuck~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		&&m_llData - data.m_llData >= LLONG_MIN)
	{
		return BigData(m_llData + data.m_llData);
	}
	//结果溢出,或者有任意一个溢出时
	return Add(data);
}

//计算几种情况，
//1至少一个参数溢出
//2两个都没有溢出，但是结果溢出。
BigData BigData::Add(const BigData & data)
{
	//将两个操作数都转化保存为string 类型，然后相加
	//1得到两个字符串，
	//2相加

	string str1 = m_strData == "" ? (INT64ToString(m_llData)) : m_strData;
	string str2 = data.m_strData == "" ? (INT64ToString(data.m_llData)) : data.m_strData;
	
	if (str1.size() < str2.size())
	{
		str1.swap(str2);
	}
	else if (str1.size() == str2.size())
	{
		//如果两个长度相等，通过一定策略改变保证--------绝对值左大于右。
		char temp = str2[0];
		str2[0] = str1[0];

		//异号且绝对值相同
		if (str1[0]!=temp&&str1[0] == str2[0] && strcmp(str1.c_str(), str2.c_str()) == 0)
		{
			return BigData(INT64(0));
		}
		if (strcmp(str1.c_str(), str2.c_str())<0)
		{
			str2[0] = temp;
			str1.swap(str2);
		}
		str2[0] = temp;

	}
	size_t sIndex1 = str1.size()-1;
	size_t sIndex2 = str2.size()-1;
	//顺序加，逆序存，
	//符号正负。

	string retData;
	//同号
	if (str1[0] == str2[0])
	{
		//符号位
		retData.append(1, str1[0]);

		int Step = 0;
		while (sIndex1)
		{
			int ret;
			if (sIndex2 != 0)
			{
				ret = str1[sIndex1] - '0' + str2[sIndex2] - '0' + Step;
				sIndex2--;
			}
			else
			{
				ret = str1[sIndex1] - '0' + Step;
			}
			Step = ret / 10;
			retData.append(1, ret % 10 + '0');
			sIndex1--;
		}

		if (Step != 0)
		{
			retData.append(1, Step + '0');
		}
	}
	//异号且绝对值左大于右
	else
	{
		retData.append(1, str1[0]);
		int Step = 0;
		while (sIndex1)
		{
			int ret;
			if (sIndex2 != 0)
			{
				ret = str1[sIndex1] - str2[sIndex2--] + Step;
			}
			else
			{
				ret = str1[sIndex1] - '0' + Step;
			}
			Step = (ret-9) / 10;
			retData.append(1, (ret+10) % 10 + '0');
			sIndex1--;
		}
	}

	
	long Size = retData.size() - 1;
	for (long begin = 1; begin <= Size; ++begin, --Size)
	{
		::swap(retData[begin], retData[Size]);
	}

	return BigData(retData.c_str());
}

BigData BigData::operator-(const BigData&data)
{
	//两个操作数都没有溢出，结果也没有溢出
	if (data.m_strData.size() == 0 && m_strData.size() == 0
		&& m_llData - data.m_llData <= LLONG_MAX
		//fuck~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		&&m_llData + data.m_llData >= LLONG_MIN)
	{
		return BigData(m_llData - data.m_llData);

	}
	//结果溢出,或者有任意一个溢出时
	return Sub(data);
}

BigData BigData::Sub(const BigData& data)
{
	BigData retData(data);
	if (retData.m_strData == "")
	{
		retData.m_llData *= -1;
	}
	else if (retData.m_strData[0] == '+')
	{
		retData.m_strData[0] = '-';
	}
	else
	{
		retData.m_strData[0] = '+';
	}
	return *this+retData;
}


BigData BigData::operator*(const BigData&data)
{
	//两个操作数都没有溢出，结果也没有溢出
	if (data.m_strData.size() == 0 && m_strData.size() == 0
		&& m_llData * data.m_llData <= LLONG_MAX
		//fuck~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		&&m_llData * data.m_llData >= LLONG_MIN)
	{
		return BigData(m_llData * data.m_llData);
	}
	//结果溢出,或者有任意一个溢出时
	return Mul(data);
}


BigData BigData::Mul(const BigData&data)
{
	//str1长度小于str2.
	string str1 = m_strData == "" ? (INT64ToString(m_llData)) : m_strData;
	string str2 = data.m_strData == "" ? (INT64ToString(data.m_llData)) : data.m_strData;

	if (str1.size() > str2.size())
	{
		str1.swap(str2);
	}
	BigData retData(INT64(0));
	char retSys = str1[0] == str2[0] ? '+' : '-';

	size_t sIndex1 = str1.size() - 1;

	int count = 0;//增加偏移0的位数
	while (sIndex1)
	{
		string tmp;
		tmp.push_back('+');
		
		tmp.append(count, '0');
		size_t sIndex2 = str2.size() - 1;

		//
		count++;
		int Step = 0;
		while (sIndex2)
		{
			//遇到0直接补位即可
			if (str1[sIndex1] == '0')
				break;

			int ret;
			ret = (str1[sIndex1] - '0') * (str2[sIndex2] - '0') + Step;
			
			Step = ret / 10;
			tmp.append(1, ret % 10 + '0');
			sIndex2--;

		}
		if (Step != 0)
		{
			tmp.append(1, Step + '0');
		}

		long Size = tmp.size() - 1;
		for (long begin = 1; begin <= Size; ++begin, --Size)
		{
			::swap(tmp[begin], tmp[Size]);
		}

		retData = retData + BigData(tmp.c_str());
		sIndex1--;
	}

	retData.m_strData[0]= retSys;
	//逆置
	return retData;
}

BigData BigData::operator/(const BigData&data)
{
	
	if (data.m_strData.size() == 0 && m_strData.size() == 0
		&& m_llData * data.m_llData <= LLONG_MAX
		//fuck~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		&&m_llData * data.m_llData >= LLONG_MIN)
	{
		return BigData(m_llData * data.m_llData);
	}
	//结果溢出,或者有任意一个溢出时
	return Div(data);
}

BigData BigData::Div(const BigData&data)
{
//	除法。
	string retData("+");
	if (data.m_llData ==  0)
	{
		cout << "非法除" << endl;
		exit(1);
	}
	else if (m_llData == 0)
	{
		return BigData(INT64(0));
	}
	
	string str1 = m_strData == "" ? (INT64ToString(m_llData)) : m_strData;
	string str2 = data.m_strData == "" ? (INT64ToString(data.m_llData)) : data.m_strData;

	//符号位确定。
	int retSys = str1[0] == str2[0] ? 1 : -1;

	//修改符号位为正保证计算方便
	str1[0] = str2[0] = '+';

	size_t sIndex1 = str1.size() - 1;
	size_t sIndex2 = str2.size() - 1;
	//如果2的长度小于1 那么返回0
	if (str1.size() < str2.size())
	{
		return BigData(INT64(0));
	}
	//如果长度相等.直接进行相减得到具体个位数。
	//长度由偏移。。正式运算
	else
	{
		BigData result(INT64(0));
		///1022 2222/9911
		//计算5次，补0  3个
		
		//除数
		string res(str1);
		//被除数
		string divNum(str2);

		while (BigData(res.c_str())>=BigData(divNum.c_str()))
		{
			//因为111/99或者99/11两种补齐不定，所以统一少一位。

			int loopTime = res.size() - divNum.size() - 1;
			string ret("+");
			//除数补0;
			if (loopTime>0)
				divNum.append(loopTime, '0');
		
			///核心1.技术分步的除结果。
			int retNum = 1;
			while (BigData(res.c_str()) >= BigData(retNum)*BigData(divNum.c_str()))
			{
				retNum = retNum+1;
			}
			//出来之后应该是比结果大1；
			//一次除结果/[0,99)；
			retNum = retNum-1;
			//if (retNum < 0)
			//	continue;
			if (retNum/10)
			{
				ret.append(1, retNum / 10 + '0');
				ret.append(1, retNum % 10 + '0');
			}
			else
			{
				ret.append(1, retNum + '0');
			}
			//相同位数时没有补0，这时loopTime为-1，不需要增加0；
			if (loopTime>0)
				ret.append(loopTime, '0');

			//结果叠加运算。。
			result = result + BigData(ret.c_str());


			BigData tmp = BigData(res.c_str()) - BigData(divNum.c_str())*BigData(INT64(retNum));
			//更新除数
			res = tmp.m_strData==""?INT64ToString(tmp.m_llData):tmp.m_strData.c_str();

			//被除数还原
			divNum = str2;
		}

		//符号位更正
		result = result*retSys;
		return result;
	}
}

bool BigData::operator>(const BigData&data)
{
	if (data.m_strData.size() == 0 && m_strData.size() == 0)
	{
		return (m_llData > data.m_llData);
	}
	//结果溢出,或者有任意一个溢出时
	BigData tmp = (*this) - data;
	if (tmp.m_strData == "")
	{
		if (tmp.m_llData >0)
			return true;
		else
			return false;
	}
	else
	{
		if (tmp.m_strData[0] == '+')
			return true;
		else
			return false;
	}
}
bool BigData::operator==(const BigData&data)
{
	if (data.m_strData.size() == 0 && m_strData.size() == 0)
	{
		return (m_llData == data.m_llData);
	}
	//结果溢出,或者有任意一个溢出时
	BigData tmp = (*this) - data;

	if (tmp.m_llData == 0)
		return true;
	else
		return false;
}


bool BigData::operator>=(const BigData&data)
{
	return (*this >data || *this == data);

}
bool BigData::operator<(const BigData&data)
{
	return !(*this >data || *this == data);
}
