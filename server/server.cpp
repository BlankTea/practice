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


			//�㲥����
			for (int i = 0; i < count; i++)
			{
				send(clientSocket[i], buff, strlen(buff), NULL); 
				
			}


		}

	}



}
int main() {
	//����Э��汾
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2),&wsaData);
	if (LOBYTE(wsaData.wVersion)!=2||HIBYTE(wsaData.wVersion)!=2 ){
		printf("����Э��汾ʧ�ܣ�\n");
		



			return -1;
	}
	printf("����Э��ɹ�");

	//����socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (SOCKET_ERROR==serverSocket ) {
		printf("����socketʧ��\n");
		WSACleanup();
		return -2;

	}
	printf("����socket�ɹ�\n");

	//����Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;


	char HostName[100];
	char *IpAddr;
	PHOSTENT hostinfo;

	gethostname(HostName, sizeof(HostName));
	hostinfo = gethostbyname(HostName);
	IpAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);


	addr.sin_addr.S_un.S_addr = inet_addr(IpAddr);//ʹ���Լ���ǰ���Ե�IP��ַ
	addr.sin_port = htons(10086);//�˿ںţ�0~65535 �˿ں����ʹ��10000���ң�����ϵͳ�ں˺����������ռ�õ���

	//�󶨵�һ��
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	//���
	if (-1 == r) {
		printf("bindʧ��\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("bind�ɹ�\n");
	//����
	r = listen(serverSocket, 10);//������������������У�����Ϊ10
	if (-1 == r) {
		printf("listenʧ��\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("listen�ɹ���\n");

	//�ȴ��ͻ������ӣ�������������(���û�пͻ������ӷ���������һֱ�ȴ�)
	
	while(1){
	clientSocket[count] = accept(serverSocket,(sockaddr*)&cAddr,&len);//���ͻ��˵�Э���ַ���������
	if (SOCKET_ERROR == clientSocket[count]) {
		printf("������崻�\n");
		//�ر�socket
		closesocket(serverSocket);
		//���Э����Ϣ
		WSACleanup();
		return -2;
	}
	printf("�пͻ������ӵ���������%s!\n", inet_ntoa(cAddr.sin_addr));//����ַ������ת��Ϊ�ַ�������
	

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)tongxin, (char*)count, NULL, NULL);
	count++;
	
	
	}

	//ͨ��

		

	return 0;
}