#define _CRT_SECURE_NO_WARNINGS

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<fstream>
#include<direct.h>

#include <process.h>  
#include <windows.h>  
#include<iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")


//创建多子个线程实例  
#include <stdio.h>  

//子线程函数  
unsigned int __stdcall ThreadFun(PVOID pM)
{
	static int i = 0;
	cout << i++ << endl;
	printf("线程ID号为%4d的子线程说：Hello World\n", GetCurrentThreadId());
	return 0;
}
//主函数，所谓主函数其实就是主线程执行的函数。  
int test()
{
	printf("     创建多个子线程实例 \n");
	printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

	const int THREAD_NUM = 5;
	HANDLE handle[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
	getchar();
	return 0;
}


bool SendFile(SOCKET& sock, string &filename)
{
	//发送缓冲
	char sendBuf[BUFSIZ] = { 0 };
	
	FILE * fp;
	fp = fopen(filename.c_str(), "rb");
	if (!fp)
		return false;

	while (1)
	{
		memset(sendBuf, 0, BUFSIZ); 
		size_t len = fread(sendBuf, 1, BUFSIZ, fp);
		printf("sendBuf:%s\n", sendBuf);		
		
		send(sock, sendBuf, len, 0);
		if (len < 1)
			break;
	}
	cout << "Send File Finished" << endl;
	fclose(fp);
	return true;
}
unsigned int __stdcall TalkWithClient(PVOID pM)
{
	SOCKET sockConn = *(SOCKET*)pM;
	int testCount = 3;
	bool testPass = false;
	//身份验证
	while (testCount--)
	{
		char buff[BUFSIZ] = "请输入用户名";
		send(sockConn, buff, strlen(buff) + 1, 0);

		char uName[BUFSIZ];
		recv(sockConn, uName, BUFSIZ, 0);
		cout << "用户输入用户名为" << uName << endl;
		char uPass[BUFSIZ];
		recv(sockConn, uPass, BUFSIZ, 0);
		cout << "用户密码为" << uPass << endl;
		if (strcmp(uName, uPass) == 0)
		{
			testPass = true;
			break;
		}
	}

	if (!testPass)//身份验证失败,关闭连接。
	{
		char buff[BUFSIZ] = "身份验证失败！";

		send(sockConn, buff, strlen(buff) + 1, 0);

		closesocket(sockConn);
		return 0;
	}

	//身份验证通过，进入正式交互过程

	char sendBuff[BUFSIZ] = "身份验证成功，欢迎使用！";
	send(sockConn, sendBuff, strlen(sendBuff) + 1, 0);

	Sleep(2000);
	char recvBuff[BUFSIZ];

	memset(recvBuff, 0, sizeof(recvBuff));
	recv(sockConn, recvBuff, BUFSIZ, 0);

	send(sockConn, "", 1, 0);
	if (strcmp(recvBuff, "文件下载请求") == 0)
	{
		//发送当前服务器文件列表
		char   buffer[MAX_PATH];
		_getcwd(buffer, MAX_PATH);
		string curdir = buffer;
		string fileList = curdir + "\\FileList\\";
		_chdir(fileList.c_str());

		//memset(buffer, 0, sizeof(buffer));
		//_getcwd(buffer, MAX_PATH);
		printf("已跳转至服务器根目录:   %s\n", buffer);

		//显示文件列表
		//ShowFileList(fileList);

		//接收欲下载文件名		
		memset(recvBuff, 0, sizeof(recvBuff));
		recv(sockConn, recvBuff, BUFSIZ, 0);
		cout << recvBuff << endl;		//用户下载文件名称

		string filename = fileList;
		filename += recvBuff;
		cout << recvBuff << endl;
		SendFile(sockConn, filename);
		closesocket(sockConn);
	}
	return 1;
}


const int THREAD_NUM = 5; //服务器最大监听数


static bool Handle_connect(int sockSrv)
{
	SOCKADDR_IN addrClient;  //保存客户端的ip地址
	int len = sizeof(SOCKADDR);

	//监听循环
	HANDLE handle[THREAD_NUM];
	int i = 0;
	while (i<THREAD_NUM)
	{
		//这里可能进程会阻塞，等待唤醒
		cout << "服务器等待客户端连接" << endl;
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //接受客户端连接
		if (sockConn > 0) // 连接成功
		{
			cout << "当前连接数为" << i + 1 << endl;
			handle[i++] = (HANDLE)_beginthreadex(NULL, 0,TalkWithClient, &sockConn, 0, NULL);
		}
	}
	WaitForMultipleObjects(THREAD_NUM, handle, true, INFINITE);
	return true;
}

int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //用阿里存储系统传回的关于WinSocket的资料
	int err;  //用来判断启动函数是否正常

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip的协议
	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = inet_addr("172.16.89.39");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(1111);  //ftp server

	//绑定自己的socket地址 到一个socket对象  等于说服务器和客户端都要进行该socket的 连接/绑定，且服务器影应该先绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//设计监听该socket,该socket已经绑定了socket地址，所以凡是连接该socket的客户端都会被加入到队列中
	listen(sockSrv, THREAD_NUM);  //设置监听队列  使服务器socket处于监听状态

	//
	Handle_connect(sockSrv);
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}
