// Server1.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	SOCKET clients[64];
	int numClients = 0;

	SOCKET registeredClients[64];
	int registered = 0;
	char * ids[64];

	int ret;

	char buf[256];

	char id[64];
	char cmd[64];
	char tmp[64];

	char errorMsg[] = "Syntax Error. Please try again.\n";

	char sendBuf[256];
	char targetID[64];
	//char okMsg[] = "Correct syntax. Type your message\n";
	while (1)
	{
		FD_ZERO(&fdread);

		//Gan cac socket vao tap fdread
		FD_SET(listener, &fdread);
		for (int i = 0; i < numClients; i++) {
			FD_SET(clients[i], &fdread);
		}
		//Tham do cac su kien bang lenh select
		ret = select(0, &fdread, NULL, NULL, NULL);

		if (ret == SOCKET_ERROR) {
			break;
		}
		if (FD_ISSET(listener, &fdread))
		{
			SOCKET client = accept(listener, NULL, NULL);
			printf("New client accepted: %d\n", client);

			clients[numClients] = client;
			numClients++;
		}

		for (int i = 0; i < numClients; i++) {
			if (FD_ISSET(clients[i], &fdread))
			{
				ret = recv(clients[i], buf, sizeof(buf), 0);
				if (ret <= 0)
					continue;

				buf[ret] = 0;
				printf("Received from %d: %s\n", clients[i], buf);

				// Kiem tra trang thai cua client
				// Va xu ly du lieu theo trang thai tuong ung

				int j = 0;
				for (; j < registered; j++) {
					if (clients[i] == registeredClients[j])
						break;
				}
				if (j == registered)
				{
					// Trang thai chua dang nhap
					// Kiem tra cu phap client_id: [id]
					ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
					if (ret == 2)
					{
						if (strcmp(cmd, "client_id:") == 0)
						{
							char okMsg[] = "Dung cu phap. Hay nhap thong diep muon gui.\n";
							send(clients[i], okMsg, strlen(okMsg), 0);

							// Luu client dang nhap thanh cong vao mang
							registeredClients[registered] = clients[i];
							ids[registered] = (char *)malloc(strlen(id) + 1);
							memcpy(ids[registered], id, strlen(id) + 1);
							++registered;
						}
						else
							send(clients[i], errorMsg, strlen(errorMsg), 0);
					}
					else
						send(clients[i], errorMsg, strlen(errorMsg), 0);
				}
				else
				{
					// Trang thai da dang nhap
					ret = sscanf(buf, "%s", targetID);
					if (ret == 1)
					{
						if (strcmp(targetID, "all") == 0)
						{
							sprintf(sendBuf, "%s: %s", ids[j], buf + strlen(targetID) + 1);

							for (int j = 0; j < registered; j++)
								if (registeredClients[j] != clients[i])
									send(registeredClients[j], sendBuf, strlen(sendBuf), 0);
						}
						else
						{
							sprintf(sendBuf, "%s: %s", ids[j], buf + strlen(targetID) + 1);

							for (int j = 0; j < registered; j++)
								if (strcmp(ids[j], targetID) == 0)
									send(registeredClients[j], sendBuf, strlen(sendBuf), 0);
						}
					}
				}

			}
		}
	}
	
	closesocket(listener);
    WSACleanup();
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
