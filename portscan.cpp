// portscan.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<WinSock.h>

#pragma comment(lib,"ws2_32")

void usage(char *prog)
{
	printf("Usage:GSGBannerScan.exe 127.0.0.1 65535\n");
	printf("GSGBannerScan.exe IP Startprot Endport\n");
}

int main(int argc, char * argv[])
{
	//查看参数
	printf("%s\n%s\n%s\n%s\n",argv[0],argv[1],argv[2],argv[3]);
	printf("arg count:%d\n",argc);
	if(argc!=4)
	{
		usage(argv[0]);
		return -1;
	}
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
	{
		printf("Winsock Dll init Failed!\n");
		return -1;
	}
	int nowport,count=0;
	struct sockaddr_in sa;
	int startport=atoi(argv[2]);
	int endport=atoi(argv[3]);
	if(endport<startport)
	{
		printf("don\'t doing,endport<startport\n");
		return -1;
	}
	nowport=startport;
	printf("Start Scan......\n");
	for(nowport;nowport<endport;nowport++)
	{
		sa.sin_family=AF_INET;
		sa.sin_addr.S_un.S_addr=inet_addr(argv[1]);
		sa.sin_port=htons(nowport);	
		SOCKET sockFD=socket(AF_INET,SOCK_STREAM,0);
		if(sockFD == INVALID_SOCKET)
		{
			printf("Socket create Error!\n");
			return -1;
		}
		int iTimeOut=5000;
		setsockopt(sockFD,SOL_SOCKET,SO_RCVTIMEO,(char *)&iTimeOut,sizeof(iTimeOut));
		if(connect(sockFD,(const sockaddr*)&sa,sizeof(sa))== SOCKET_ERROR)
		{
			closesocket(sockFD);
		}else{
			count++;
			printf("%s Find %d Port is Opend!\n",argv[1],nowport);
			if(nowport==21)
			{
				char buff[2048]={0};
				char hello[5]={"test"};
				send(sockFD,hello,sizeof(hello),0);
				recv(sockFD,buff,sizeof(buff),0);
				printf("FTP Banner:%s\n",buff);
			}else{
				if(nowport==80)
				{
					char buff[2048]={0};
					char get[30]={"GET HTTP 1.0/1.1\n\n\r\r"};
					send(sockFD,get,sizeof(get),0);
					recv(sockFD,buff,sizeof(buff),0);
				}
			}
			closesocket(sockFD);
		}
	}
	printf("Scan End......\n Find %d Port is Opened!\n",count);
	WSACleanup();
	return 0;
}

