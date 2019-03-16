// TelnetServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "winsock2.h"


int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);

	fd_set fdread;
	int ret;

	SOCKET clients[64];
	int numClients = 0;

	char buf[256];
	char fileBuf[256];
	char cmdBuf[256];

	while (true)
	{
		FD_ZERO(&fdread);
		FD_SET(listener, &fdread);
		for (int i = 0; i < numClients; i++)
			FD_SET(clients[i], &fdread);

		ret = select(0, &fdread, NULL, NULL, NULL);

		if (ret == SOCKET_ERROR)
			break;

		if (ret > 0)
		{
			// Xu ly su kien co ket noi moi
			if (FD_ISSET(listener, &fdread))
			{
				SOCKET client = accept(listener, NULL, NULL);
				printf("New client accepted: %d\n", client);
				
					ret = recv(client, buf, sizeof(buf), 0);
					buf[ret] = 0;
					printf("%s\n", buf);
					int found = 0;
					FILE *f = fopen("users.txt", "r");
					while (fgets(fileBuf, sizeof(fileBuf), f))
					{
						printf("%s\n", fileBuf);
						if (strcmp(buf, fileBuf) == 0)
						{
							found = 1;
							break;
						}
					}
					fclose(f);

					if (found == 1)
					{
						char msg[] = "Dang nhap thanh cong. Hay nhap lenh.\n";
						send(client, msg, strlen(msg), 0);
						//break;
					}
					else
					{
						char msg[] = "Dang nhap that bai. Hay thu lai.\n";
						send(client, msg, strlen(msg), 0);
					}
					
					clients[numClients] = client;
					numClients++;
				
				printf("HEY!");
				ret = recv(client, buf, sizeof(buf), 0);
			}

			// Xu ly su kien khi co du lieu den cac client
			for (int i = 0; i < numClients; i++) {

				if (FD_ISSET(clients[i], &fdread))
				{
					printf("rferg");
					ret = recv(clients[i], buf, sizeof(buf), 0);

					if (ret <= 0)
					{
						// Ket noi bi huy
						continue;
					}

					buf[ret] = 0;
					if (buf[ret - 1] =='\n')
					{
						buf[ret - 1] = 0;
					}
					printf("Received from %d: %s\n", clients[i], buf);

					sprintf(cmdBuf, "%s > out1.txt", buf);
					system(cmdBuf);

					FILE *f = fopen("out.txt", "r");
					while (fgets(fileBuf, sizeof(fileBuf), f))
					{
						send(clients[i], fileBuf, strlen(fileBuf), 0);
					}
					fclose(f);
				}
			}
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
