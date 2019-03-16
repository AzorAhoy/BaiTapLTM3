// Client1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	connect(client, (SOCKADDR *)&addr, sizeof(addr));

	fd_set fdread;
	int ret;

	char buf[256];

	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	while (true)
	{
		// Khoi tao trang thai tap fdread
		FD_ZERO(&fdread);

		// Gan socket voi tap fdread
		FD_SET(client, &fdread);

		// Tham do su kien bang lenh select
		ret = select(0, &fdread, NULL, NULL, &tv);

		if (ret == SOCKET_ERROR)
		{
			// Loi => Ket thuc chuong trinh
			break;
		}
		if (ret == 0)
		{
			printf("Timed Out!\n");
		}
		else if (ret > 0)
		{
			ret = recv(client, buf, sizeof(buf), 0);

			buf[ret] = 0;
			printf("Received: %s\n", buf);

			printf("Nhap du lieu: ");
			gets_s(buf, sizeof(buf));
			//char * pch;
			//pch = strchr(buf, ':');
			//if (pch != NULL)
			//{
			//	printf("found at %d\n", pch - buf + 1);
			//	//pch = strchr(pch + 1, ':');
			//	
			//}
			send(client, buf, strlen(buf), 0);


			if (strncmp(buf, "exit", 4) == 0)
				break;
		}
	}

	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
