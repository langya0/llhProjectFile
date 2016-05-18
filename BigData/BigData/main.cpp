#include"bigdata.h"

//void ()
//{
//	string t1("+9223372036854775807");
//	string t2("+12456778675645342345676544323456765544");
//	cout << (t1 > t2) << endl;
//}
void TestInit()
{
	string tet("-9223372036854775808");
	cout << tet.size() << endl;

	BigData big1("+123");
	BigData big2("+123");
	BigData big3("+123asdf");
	BigData big4("+00012sdf3");
	BigData big5("    -12wer3");
	BigData big6("+12456778675645342345676544323456765544");
	BigData big7("-12456778675645342345676544323456765544");
	//big.INT64ToString();
	cout << big1;
	cout << big2;
	cout << big3;
	cout << big4;
	cout << big5;
	cout << big6;
	cout << big7;
}

void TestAdd()
{
	BigData big1("+123");
	BigData big2("-123");


	//cout << -12 / 10 << endl;
	//cout << -12 % 10 << endl;
	cout << (big1 + big2) << endl;

	BigData big3("+1111111111111111111111111111111234");
	BigData big4("-1111111111111111111111111111111234");
	cout << (big3 + big4) << endl;

	BigData big5("+1");
	BigData big6("+99999999999999999999999999999999999999432");
	cout << (big5 + big6) << endl;

	BigData big7("+111111111111111111111111111111111");
	BigData big8("-6666666666666666666666666666666666666666666666666666666666666666");
	cout << (big7 + big8) << endl;

	BigData big39("+0");
	BigData big49("+99999999999999999999999999999999999999432");
	cout << (big39 + big49) << endl;
}

void TestSub()
{
	BigData big1("-123");
	BigData big2("-1111111111111111111111111111111111");


	//cout << -12 / 10 << endl;
	//cout << -12 % 10 << endl;
	cout << (big2 + big1) << endl;

	BigData big3("+1111111111111111111111111111111234");
	BigData big4("-1111111111111111111111111111111234");
	cout << (big3 - big4) << endl;

	BigData big5("+1");
	BigData big6("+99999999999999999999999999999999999999432");
	cout << (big5 - big6) << endl;


	BigData big7("+111111111111111111111111111111111");
	BigData big8("+66666666666666666666666666666666");
	cout << (big7 - big8) << endl;


	BigData big39("+0");
	BigData big49("+99999999999999999999999999999999999999432");
	cout << (big39 - big49) << endl;

	cout << -123 - 111 << endl;
}

void TestMul()
{
	BigData big1("-0");
	BigData big2("-10");


	//cout << -12 / 10 << endl;
	//cout << -12 % 10 << endl;
	cout << (big2 * big1) << endl;

	BigData big3("+111");
	BigData big4("-1111111111111111111111111111111");
	cout << (big3 * big4) << endl;

	BigData big5("+1");
	BigData big6("+99999999999999999999999999999999999999432");
	cout << (big5 * big6) << endl;

	BigData big7("+111111111111111111111111111111111");
	BigData big8("-666666666666666666666666");
	cout << (big7 * big8) << endl;

	BigData big39("+0");
	BigData big49("+99999999999999999999999999999999999999432");
	cout << (big39 * big49) << endl;
}


void main()
{
	//BigData big1("-0");
	//BigData big2("-10");


	////cout << -12 / 10 << endl;
	////cout << -12 % 10 << endl;
	//cout << (big2 / big1) << endl;

	//BigData big3("+111");
	//BigData big4("-1111111111111111111111111111111");
	//cout << (big3 / big4) << endl;
	//   
	//BigData big5("+1");
	//BigData big6("+99999999999999999999999999999999999999432");
	//cout << (big5 / big6) << endl;

	BigData big7("+99999999999999999999999999999999");
	BigData big8("-111111111111119211111111111111111");
	cout << big7 << endl;
	cout << big8 << endl;
	cout << (big7 / big8)<< endl;

	//BigData big39("+0");
	//BigData big49("+99999999999999999999999999999999999999432");
	//cout << (big39 / big49) << endl;
}
