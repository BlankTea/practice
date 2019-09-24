#include<stdio.h>
#include<Windows.h>


#pragma comment(lib,"ws2_32.lib")

SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;

SOCKET clientSocket[1024];
int count = 0;


void tongxin(int idx) {

	char buff[1024];
	int r;
	
	while (1) {
		r = recv(clientSocket[idx],buff,1023,NULL);
		if (r > 0) {
			buff[r] = 0;
			printf("%d:%s\n", idx, buff);


			//广播数据
			for (int i = 0; i < count; i++)
			{
				send(clientSocket[i], buff, strlen(buff), NULL); 
				
			}


		}

	}



}
int main() {
	//请求协议版本
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2),&wsaData);
	if (LOBYTE(wsaData.wVersion)!=2||HIBYTE(wsaData.wVersion)!=2 ){
		printf("请求协议版本失败！\n");
		



			return -1;
	}
	printf("请求协议成功");

	//创建socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SOCKET_ERROR==serverSocket ) {
		printf("创建socket失败\n");
		WSACleanup();
		return -2;

	}
	printf("创建socket成功\n");

	//创建协议地址族
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;


	char HostName[100];
	char *IpAddr;
	PHOSTENT hostinfo;

	gethostname(HostName, sizeof(HostName));
	hostinfo = gethostbyname(HostName);
	IpAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);


	addr.sin_addr.S_un.S_addr = inet_addr(IpAddr);//使用自己当前电脑的IP地址
	addr.sin_port = htons(10086);//端口号：0~65535 端口号最好使用10000左右，操作系统内核和其他程序会占用掉。

	//绑定到一起
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	//检查
	if (-1 == r) {
		printf("bind失败\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("bind成功\n");
	//监听
	r = listen(serverSocket, 10);//多个服务器安队列排列，长度为10
	if (-1 == r) {
		printf("listen失败\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("listen成功！\n");

	//等待客户端连接，采用阻塞函数(如果没有客户端连接服务器，则一直等待)
	
	while(1){
	clientSocket[count] = accept(serverSocket,(sockaddr*)&cAddr,&len);//将客户端的协议地址族放在这里
	if (SOCKET_ERROR == clientSocket[count]) {
		printf("服务器宕机\n");
		//关闭socket
		closesocket(serverSocket);
		//清除协议信息
		WSACleanup();
		return -2;
	}
	printf("有客户端连接到服务器：%s!\n", inet_ntoa(cAddr.sin_addr));//将地址从整型转换为字符串类型
	

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)tongxin, (char*)count, NULL, NULL);
	count++;
	
	
	}

	//通信

		

	return 0;
}