
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>
#include<graphics.h>

#pragma comment(lib,"ws2_32.lib")




SOCKET clientSocket;
HWND hWnd;
int count = 0;




void jieshou() {
	
	char recvBuff[1024];
	int r;
	while (1) {
		r = recv(clientSocket, recvBuff,1023, NULL);
		if (r > 0) {
			recvBuff[r] = 0;
			outtextxy(0, count * 20, recvBuff);
			count++;

		}


	}


}





int main() {

	//初始化界面

	hWnd = initgraph(300, 400, SHOWCONSOLE);
	


	//客户端
	//请求协议版本
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion )!= 2|| HIBYTE(wsaData.wVersion) != 2) {
		printf("请求协议版本失败！\n");

		return -1;
	}
	printf("请求协议成功");

	//创建socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SOCKET_ERROR == clientSocket) {
		printf("创建socket失败\n");
		WSACleanup();
		return -2;

	}
	printf("创建socket成功\n");

	//获取协议地址族
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;



	//	获取本机IP
	char HostName[100];
	char *IpAddr;
	PHOSTENT hostinfo;

	gethostname(HostName, sizeof(HostName));
	hostinfo = gethostbyname(HostName);
	IpAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);



	
	addr.sin_addr.S_un.S_addr = inet_addr(IpAddr);//使用自己当前电脑的IP地址

	addr.sin_port = htons(10086);//端口号：0~65535 端口号最好使用10000左右，操作系统内核和其他程序会


	//连接服务器
	int r = connect(clientSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		printf("连接服务器失败\n");
		return -1;

	}
	printf("连接服务器成功\n");

		
	//通信

	char buff[1024];

	CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE) jieshou, NULL, NULL, NULL);



	while (1) {
		memset(buff, 0, 1024);
		printf("你想说什么：");
		scanf("%s", buff);/*scanf("%s", buff);*/
		 send(clientSocket, buff, strlen(buff), NULL);
	}
	return 0;
}