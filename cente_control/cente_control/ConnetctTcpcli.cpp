#include "ConnetctTcpcli.h"
#include "other.h"

SOCKET connectTcpCli()
{
CONTINUE:
	FILE *fd_IP = fopen("serIP.txt", "r+");
	FILE *fd_Port = fopen("serport.txt", "r+");
	char serIP[20] = { 0 };
	char serPort[6] = { 0 };
	fscanf(fd_IP, "%s", serIP);
	fscanf(fd_Port, "%s", serPort);

    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data; 
    if(WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        printf("invalid socket !");
        return INVALID_SOCKET;
    }
	   
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(charToShort(serPort));
    serAddr.sin_addr.S_un.S_addr = inet_addr(serIP); 
    if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("connect error !\n");
        closesocket(sclient);
		fclose(fd_IP);
		fclose(fd_Port);
		Sleep(1000);
		goto CONTINUE;
        return SOCKET_ERROR;
    }
	fclose(fd_IP);
	fclose(fd_Port);
    return sclient;
}

SOCKET connectTcpSer()
{

	 //��ʼ��WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        return 0;
    }

    //�����׽���
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    //��IP�Ͷ˿�
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY; 
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //��ʼ����
    if(listen(slisten, 3) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }

   return slisten;
    
   
}
//closesocket(sClient);
 //closesocket(slisten);
   // WSACleanup();
/*
 //ѭ����������
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255]; 
    while (true)
    {
        printf("�ȴ�����...\n");
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if(sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }
        printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
        
        //��������
        int ret = recv(sClient, revData, 255, 0);        
        if(ret > 0)
        {
            revData[ret] = 0x00;
            printf(revData);
        }

        //��������
        char * sendData = "��ã�TCP�ͻ��ˣ�\n";
        send(sClient, sendData, strlen(sendData), 0);
        
    }
*/