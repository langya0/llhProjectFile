////客户端
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
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

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip的协议
															 //初始化连接

	SOCKADDR_IN addrSrv;  //服务器的地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr("172.16.89.39");
	addrSrv.sin_family = AF_INET;  //使用的是TCP/IP 
	addrSrv.sin_port = htons(1111);  //转为网络序  设置端口号


									 //连接到服务器 使用SOCKET对象连接服务器,送入服务器的地址信息  强转
	if (connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)  //协议参数  套接字参数
	{
		printf("connction faild!");
		closesocket(socketClient);
		return 0;
	}

	//如果连接到了  那就接收
	char buff[BUFSIZ] = { 0 };
	char recvBuf[BUFSIZ] = { 0 };

	///命令解析

	int len = recv(socketClient, buff, BUFSIZ, 0);
	printf("%s\n", buff);
	//解析结果为身份验证

	char Username[BUFSIZ] = "liang";
	send(socketClient, Username, strlen(Username) + 1, 0);
	char Pass[BUFSIZ] = "liang";
	send(socketClient, Pass, strlen(Pass) + 1, 0);

	memset(buff, 0, sizeof(buff));
	recv(socketClient, buff, BUFSIZ, 0);
	cout << buff << endl;

	char req[BUFSIZ] = "文件下载请求";
	send(socketClient, req, strlen(req) + 1, 0);
	recv(socketClient, recvBuf, BUFSIZ, 0);

	////////////////////////////////////////////////
	//接收文件列表


	//选择下载文件名
	char filename[BUFSIZ] = "tt1.txt";

	send(socketClient, filename, strlen(filename) + 1, 0);
	//接收并且保存该文件到本地目录
	FILE* fp = fopen("E:\\tt1.txt", "wb");

	if (!fp)
		return 0;

	//写入文件
	while (1)
	{
		int len = recv(socketClient, recvBuf, BUFSIZ, 0);
		fwrite(recvBuf, 1, len, fp);
		printf("%s\n", recvBuf);
		memset(recvBuf, 0, BUFSIZ);

		if (len < 1)
			break;
	}
	fclose(fp);
	closesocket(socketClient);  //关闭socket连接

	WSACleanup();

	printf("Client exit!");
	system("pause");
	return 0;

}

