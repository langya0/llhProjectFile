#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
#include<fstream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")



#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<mmsystem.h>  
#include<string>
#include<time.h>
#include<fstream>
#include<stdio.h>
#include<vector>
#include<string>
#pragma comment (lib, "winmm.lib")


void ShowFileList(string filename, SOCKET sockConn)
{

	WIN32_FIND_DATAA p;
	HANDLE h = FindFirstFileA(filename.c_str(), &p);
	if (string(p.cFileName) != "." || string(p.cFileName) != "..")
	{
		send(sockConn, p.cFileName, 260, 0);
	}
	while (FindNextFileA(h, &p))
	{
		if (string(p.cFileName) != "." || string(p.cFileName) != "..")
		{
			cout << p.cFileName << endl;
			send(sockConn, p.cFileName, 260, 0);
		}
	}
	char end[260] = "transform end";
	send(sockConn, "transform end", 260, 0);
	/*for (int i = 0; i < filelist.size(); i++)
	{
	cout << filelist[i] << endl;
	}*/
}

void ShowLastFileList(string & filepath, SOCKET sockConn)
{
	string filepath2 = filepath;
	string tmp = "../";
	tmp += filepath2;
	filepath = tmp;
	ShowFileList(tmp, sockConn);
}


void ShowSelectFileList(string &filepath, SOCKET sockConn)
{
	char Path[260] = { 0 };
	recv(sockConn, Path, 260, 0);
	cout << "Path:" << Path << endl;
	string yourchoose(Path);
	yourchoose += '/';
	string filepath2 = filepath;
	yourchoose += filepath2;
	ShowFileList(yourchoose, sockConn);
}



string ExecuteCommand(SOCKET sockConn)
{
	string filepath = "";
	string FileName = "*.*";
	string FilePath = filepath + FileName;
	while (1)
	{
		char *command = new char(0);
		recv(sockConn, command, 1, 0);
		cout << "command" << command << endl;
		if (*command == '4')
		{
			char filename[260] = { 0 };
			recv(sockConn, filename, 260, 0);
			cout << "FilePath1:  " << FilePath << endl;
			FilePath.erase(FilePath.size() - 3, 3);
			cout << "FilePath2:  " << FilePath << endl;
			FilePath += filename;
			break;
		}
		switch (*command)																  //首先发文件名
		{
		case '1':
			ShowFileList(FilePath, sockConn);
			break;
		case '2':
			ShowLastFileList(FilePath, sockConn);
			break;
		case '3':
			ShowSelectFileList(FilePath, sockConn);
			break;
		case '5':
			closesocket(sockConn);
			exit(0);
			break;
		}
	}
	return FilePath;
}





int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //用阿里存储系统传回的关于WinSocket的资料
	int err;  //用来判断启动函数是否正常

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip的协议

	SOCKADDR_IN addrSrv;

	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8001);  //ftp server

	//绑定自己的socket地址 到一个socket对象  等于说服务器和客户端都要进行该socket的 连接/绑定，且服务器影应该先绑定
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//设计监听该socket,该socket已经绑定了socket地址，所以凡是连接该socket的客户端都会被加入到队列中
	listen(sockSrv, 5);  //设置监听队列  使服务器socket处于监听状态

	SOCKADDR_IN addrClient;  //保存客户端的ip地址
	int len = sizeof(SOCKADDR);

	//监听循环
	while (1)
	{
		//这里可能进程会阻塞，等待唤醒
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);  //接受客户端连接
		//当收到一个请求时 发送给其文件


		//首先接收一个命令
		string FilePath = ExecuteCommand(sockConn);
		cout << "return FilePath:  " << FilePath << endl;
		//	char filename[256] = { "test2.txt " };  //不超过255个字符

		//		send(sockConn, filename, strlen(filename) + 1, 0);

		//send(sockConn, "hello", 6, 0);
		//Sleep(2000);
		FILE * fp;
		fp = fopen(FilePath.c_str(), "rb");
		if (!fp)
			return 0;


		//发送缓冲
		char sendBuf[512] = { 0 };  //一次512B 数据

		//FILE * fp = fopen("F:\\1.pdf", "rb");
		int count = 0;


		while (1)
		{
			memset(sendBuf, 0, 512);
			int len = fread(sendBuf, 1, 512, fp);
			send(sockConn, sendBuf, len, 0);
			//	printf("sendBuf:%c\n",* sendBuf);
			//	printf("len:%d\n", len);
			count++;
			if (len<1)
				break;
		}
		//memset(sendBuf, 0, 1);
		//strcpy(sendBuf, "end");
		//send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		//printf("Finished!\n%d",count);
		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

//客户端

#define	_CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>;
#include<vector>
#include<string>
using namespace std;

void MainMenu()
{
	printf("请选择操作\n");
	printf("1.显示当前文件夹的所有文件\n");
	printf("2.返回上一级\n");
	printf("3.选择文件夹\n");
	printf("4.输入要下载的文件\n");
	printf("5.退出\n");
}

void Showfilelist(vector<string>& filelist)
{
	for (int i = 0; i < filelist.size(); ++i)
	{
		cout << filelist[i] << endl;
	}

}

string ExecuteCommand(SOCKET &socketClient)
{
	string filename;
	char *command = new char(0);
	while (1)
	{
		system("CLS");
		MainMenu();
		scanf("%d", command);
		getchar();
		*command += '0';
		send(socketClient, command, 1, 0);
		vector<string> filelist;
		//然后就得接收
		string Path;
		if (*command == '3')
		{
			cin >> Path;
			send(socketClient, (char *)Path.c_str(), Path.size(), 0);
		}

		if (*command == '4')
		{

			cin >> filename;
			send(socketClient, (char *)filename.c_str(), filename.size(), 0);

			break;
		}
		char Buf[260] = { 0 };
		while (1)
		{
			recv(socketClient, Buf, 260, 0);
			filelist.push_back(Buf);
			memset(Buf, 0, 100);
			if (filelist.back() == "transform end")
			{
				filelist.pop_back();
				break;
			}

		}
		Showfilelist(filelist);
		system("pause");
	}
	return filename;
}

#pragma comment(lib, "ws2_32.lib")
int main()
{
	WORD wVersionRequested;  //typedef unsigned short WORD
	WSADATA wsaData;   //用阿里存储系统传回的关于WinSocket的资料
	int err;  //用来判断启动函数是否正常

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET tcpip的协议
	//初始化连接
	SOCKADDR_IN addrSrv;  //服务器的地址
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;  //使用的是TCP/IP 
	addrSrv.sin_port = htons(8001);  //转为网络序  设置端口号

	//连接到服务器 使用SOCKET对象连接服务器,送入服务器的地址信息  强转
	if (connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) < 0)  //协议参数  套接字参数 
	{
		printf("connction faild!");
		closesocket(socketClient);
		return 0;
	}


	//如果连接到了  那就先发送一个命令
	string filename = ExecuteCommand(socketClient);

	//char filename[256] = { 0 };
	char recvBuf[512] = { 0 };

	//int len = recv(socketClient, filename, 256, 0);

	//printf("%d\n", len);


	//printf("%s\n", filename);
	//创建文件
	FILE* fp = fopen(filename.c_str(), "wb");
	cout << "成功打开文件" << filename << endl;
	if (!fp)
		return 0;


	//写入文件
	while (1)
	{
		cout << "传输中" << endl;
		int len = recv(socketClient, recvBuf, 512, 0);
		cout << "接收" << endl;
		fwrite(recvBuf, 1, len, fp);
		cout << "书写" << endl;
		memset(recvBuf, 0, 512);
		cout << "清零" << endl;
		if (len < 1)
		{
			cout << "退出" << endl;
			break;
		}
	}
	printf("文件传输完成\n");




	//char recvBuf[50] = {0};

	//int size = 10;
	//int i = 0;
	//while (i<100)
	//{
	//	//recv(socketClient, recvBuf, 50, 0);  //socket对象已经接收到数据，现在开始次on个缓存区中取数据
	//    //printf("%s\n", recvBuf);
	//	send(socketClient, "123456789" , 50, 0);
	//	//Sleep(100);
	//}



	closesocket(socketClient);  //关闭socket连接

	WSACleanup();

	printf("Client exit!");
	system("pause");
	return 0;

}
