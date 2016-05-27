//#include<iostream>
//using namespace  std;
//
//class Person
//{
//public:
//	void DisPlay()
//	{
//		cout << _name << endl;
//		cout << _sex << endl;
//		cout << _age << endl;
//	}
//	char *_name;
//	char *_sex;
//	int _age;
//};
//
//int main()
//{
//	Person p1;
//	Person *pp1 = &p1;
//	p1._age = 100;
//	p1._name = "zhang";
//	pp1->_sex = "man";
//
//	p1.DisPlay();
//
//	Person p2;
//	cout << (void Person*)p1.DisPlay << endl;
//	cout << (void*)p2.DisPlay << endl;
//
//	return 0;
//}
#include<iostream>
using namespace std;
double Div(double a,double b)
{
	if(0==b)
	{
		throw "this is ziro";
	}
	return a/b;
}
int main()
{
	double a = 1.1;
	double b = 0;
	try
	{
		double ret = Div(a,b);
	}
	catch(char * pMsg)
	{
		cout << "res = " << pMsg << endl;
	}
	return 0;
}