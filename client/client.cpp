
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

	//��ʼ������

	hWnd = initgraph(300, 400, SHOWCONSOLE);
	


	//�ͻ���
	//����Э��汾
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion )!= 2|| HIBYTE(wsaData.wVersion) != 2) {
		printf("����Э��汾ʧ�ܣ�\n");

		return -1;
	}
	printf("����Э��ɹ�");

	//����socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SOCKET_ERROR == clientSocket) {
		printf("����socketʧ��\n");
		WSACleanup();
		return -2;

	}
	printf("����socket�ɹ�\n");

	//��ȡЭ���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;



	//	��ȡ����IP
	char HostName[100];
	char *IpAddr;
	PHOSTENT hostinfo;

	gethostname(HostName, sizeof(HostName));
	hostinfo = gethostbyname(HostName);
	IpAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);



	
	addr.sin_addr.S_un.S_addr = inet_addr(IpAddr);//ʹ���Լ���ǰ���Ե�IP��ַ

	addr.sin_port = htons(10086);//�˿ںţ�0~65535 �˿ں����ʹ��10000���ң�����ϵͳ�ں˺����������


	//���ӷ�����
	int r = connect(clientSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		printf("���ӷ�����ʧ��\n");
		return -1;

	}
	printf("���ӷ������ɹ�\n");

		
	//ͨ��

	char buff[1024];

	CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE) jieshou, NULL, NULL, NULL);



	while (1) {
		memset(buff, 0, 1024);
		printf("����˵ʲô��");
		scanf("%s", buff);/*scanf("%s", buff);*/
		 send(clientSocket, buff, strlen(buff), NULL);
	}
	return 0;
}