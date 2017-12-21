#include "ConnetctTcpcli.h"
#include "cJSON.h"
#include <iostream>
#include <thread>
#include <WINSOCK2.H>
#include <STDIO.H>
#include <string.h>

#include <Iphlpapi.h>
#include <time.h>

#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��
#pragma  comment(lib,"ws2_32.lib")

using namespace std;

void thread01(SOCKET sclient);
void thread02(SOCKET sclient);

void thread03(SOCKET slisten);
char *serchIPMac();


int main(int argc, char* argv[])
{

	SOCKET slisten = connectTcpSer();

	thread task03(thread03, slisten);
	
	SOCKET sclient = connectTcpCli();
	//������
	thread task01(thread01,sclient);
	//̽����͹ػ���

	task01.join();
	task03.join();
	
	system("pause");
}
void thread01(SOCKET sclient)//������
{
	while (1)
	{
		char * recvData = serchIPMac();//�õ�mac��ip��ַ
	
		cJSON *json = cJSON_Parse(recvData);
		cJSON *macnode = cJSON_GetObjectItem(json,"mac");
		cJSON *ipnode = cJSON_GetObjectItem(json,"ip");
		char sendData[100] = {0};
		sprintf(sendData,"{\"code\":\"200\",\"mac\":\"%s\",\"ip\":\"%s\"}",macnode->valuestring,ipnode->valuestring);
		printf("--------------------��������������������������\n");
		printf("%s\n",sendData);
		send(sclient, sendData, strlen(sendData), 0);
		//��������
	
		Sleep(10);
		//char recvBuf[100] = { 0 };
		//if (recv(sclient, recvBuf, sizeof(recvBuf), 0) <= 0)
		//{
		//	sclient = connectTcpCli();
		//}
		


		Sleep(45000);

		free(recvData);
		recvData = NULL;
	}
}

void thread03(SOCKET slisten)
{
	

	time_t timestamp;
    char *serIP = NULL;
	char *serPort = NULL;
	
	
	//ѭ����������
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255]; 
	int i;
    
        
		while (1)
		{
			//��������
			printf("�ȴ�����...\n");
			sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
			if (sClient == INVALID_SOCKET)
			{
				printf("accept error !");

			}
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

			FILE *fd_IP = fopen("serIP.txt", "r+");
			FILE *fd_Port = fopen("serport.txt", "r+");
			int ret = recv(sClient, revData, 255, 0);
			if (ret > 0)//���յ�һ��̽�������ȡIP��
			{
				revData[ret] = 0x00;
				printf(revData);// "{\"code\":\"200\",\"ip\":\"  \",mac,key}"
				printf("\n");
				printf("_____\n");
				
				cJSON *json = cJSON_Parse(revData);
				cJSON *codenode = NULL;
				cJSON *keynode = NULL;
				cJSON *ipnode = NULL;
				cJSON *portnode = NULL;

				codenode = cJSON_GetObjectItem(json, "code");
				keynode = cJSON_GetObjectItem(json, "key");

				if (strcmp("tr#Ya68x-beC", keynode->valuestring) == 0)
				{					
					printf("---������ȷ----\n");
					if (0 == strcmp(codenode->valuestring, "100"))
					{
						
						ipnode = cJSON_GetObjectItem(json, "ip");
						portnode = cJSON_GetObjectItem(json, "port");

						serIP = ipnode->valuestring;
						serPort = portnode->valuestring;
						
						fprintf(fd_Port, "%s", serPort);
						fprintf(fd_IP,"%s          ",serIP);
						printf("----̽���----\n");
						/*
						if (strlen(serIP) < 15)
						{
							fprintf(fd_IP, "%s", serIP);
							for (i = 0; i < (15 - strlen(serIP)); i++)
							{
								fseek(fd_IP, strlen(serIP) + i, SEEK_SET);
								fprintf(fd_IP, " ");
							}
						}
						else
						{
							fprintf(fd_IP, "%s", serIP);
						}
						*/
						printf(serIP);
						printf("\n");
					
						//��ȡip��mac
						char LocalIP[20] = { 0 };
						char Mac[20] = { 0 };
						
						char *MacIp = serchIPMac();
						if (0 == strcmp(MacIp, "not internet"))
						{
							printf("����û���\n");
						}

						cJSON *jsonMacIp = cJSON_Parse(MacIp);
						cJSON *macnode = cJSON_GetObjectItem(jsonMacIp, "mac");
						cJSON *IPnode = cJSON_GetObjectItem(jsonMacIp, "ip");
						strcpy(Mac, macnode->valuestring);
						strcpy(LocalIP, IPnode->valuestring);
						//����mac��ip
						char sendData[100] = { 0 };
						
						sprintf(sendData, "{\"code\":\"100\",\"ip\":\"%s\",\"mac\":\"%s\"}",
							LocalIP, Mac );
						printf("sendbuf is %s\n", sendData);
						send(sClient, sendData, strlen(sendData), 0);
						cJSON_Delete(jsonMacIp);

					}
					else if(0 == strcmp(codenode->valuestring, "300"))
					{
						//
						//�ػ���
						printf("------�ػ���--------\n");
						
						char * recvData = serchIPMac();//�õ�mac��ip��ַ

						cJSON *gjson = cJSON_Parse(recvData);
						cJSON *macnode = cJSON_GetObjectItem(gjson, "mac");
						cJSON *ipnode = cJSON_GetObjectItem(gjson, "ip");
						char sendBuf[100] = { 0 };
						sprintf(sendBuf, "{\"code\":\"300\",\"mac\":\"%s\",\"ip\":\"%s\"}",
							macnode->valuestring, ipnode->valuestring);
						send(sClient, sendBuf, strlen(sendBuf), 0);
						printf("_______________________\nshutdown\n");
						cJSON_Delete(gjson);
						system("shutdown -s");
					}
					else
					{
						printf("code error\n");
					}
				}
				else 
				{
					printf("�������1\n");
				}
				
				cJSON_Delete(json);
				
			}

			fclose(fd_IP);
			fclose(fd_Port);
			
		}
		closesocket(slisten);

}
char *serchIPMac()
{
	char *sendbuf = (char*)malloc(sizeof(char) * 50);
	memset(sendbuf, 0, sizeof(sendbuf));

	char macBuf[20] = { 0 };
	char ipBuf[20] = { 0 };

	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{

			//���������ж�IP,���ͨ��ѭ��ȥ�ж�
			IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{
				if (0 == strcmp(pIpAdapterInfo->GatewayList.IpAddress.String, "192.168.1.1"))
				{
					for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
					{
						char temp[5] = { 0 };
						if (i == pIpAdapterInfo->AddressLength - 1)
						{
							sprintf(temp, "%2X", pIpAdapterInfo->Address[i]);
							if (' ' == temp[0])
							{
								temp[0] = '0';
							}
							strcat(macBuf, temp);
						}
						else
						{
							sprintf(temp, "%2X-", pIpAdapterInfo->Address[i]);
							if (' ' == temp[0])
							{
								temp[0] = '0';
							}
							strcat(macBuf, temp);
						}

					}
					strcat(ipBuf, pIpAddrString->IpAddress.String);
				}

				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
			//���������ж�IP,���ͨ��ѭ��ȥ�ж�       
		}

	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
	if (0 == strlen(sendbuf))
	{
		strcpy(sendbuf, "not internet");
	}

	sprintf(sendbuf, "{\"mac\":\"%s\",\"ip\":\"%s\"}", macBuf, ipBuf);
	return sendbuf;
}