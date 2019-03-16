// Server1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "winsock2.h"


char * ids[64];

int main()
{

	char buf[256];
	char sendBuf[256];
	int ret;
	

	char cmd[64];
	
	char tmp[64];

	char targetID[64];
	char okMsg[] = "Correct syntax. Type your message\n";
	char errorMsg[] = "Syntax Error. Please try again.\n";

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
	//int ret;

	SOCKET clients[64];
	int numClients = 0;

	//char buf[256];

	while (true)
	{
		char id[64];
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
				while (true)
				{
					ret = recv(client, buf, sizeof(buf), 0);

					buf[ret] = 0;
					printf("Received from %d: %s\n", client, buf);
					ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
					if (ret == 2)
					{
						if (strcmp(cmd, "client_id:") == 0)
						{

							send(client, okMsg, strlen(okMsg), 0);
							clients[numClients] = client;
							ids[numClients] = id;
							numClients++;
							for (int i = 0; i < numClients; i++)
							{
								printf("%s\n", ids[i]);
							}
							break;
						}
						else
						{
							send(client, errorMsg, strlen(errorMsg), 0);
						}
					}
					else
					{
						send(client, errorMsg, strlen(errorMsg), 0);
					}
				}
	
			}

			// Xu ly su kien khi co du lieu den cac client
			for (int i = 0; i < numClients; i++)
				if (FD_ISSET(clients[i], &fdread))
				{
					ret = recv(clients[i], buf, sizeof(buf), 0);

					if (ret <= 0)
					{
						// Ket noi bi huy
						continue;
					}

					buf[ret] = 0;
					printf("Received from %d: %s\n", clients[i], buf);
					ret = sscanf(buf, "%s", targetID);
					sprintf(sendBuf, "%s: %s", id, buf);
					for (int j = 0; j < numClients; j++)
					{
						if (clients[i] != clients[j])
						{
							send(clients[j], sendBuf, strlen(sendBuf), 0);
						}
						printf("Received from %d: %s\n", clients[i], buf);
					}
					//if (ret == 1)
					//{
					//	if (strcmp(targetID, "all") == 0)
					//	{
					//		sprintf(sendBuf, "%s %s", id, buf + strlen(targetID) + 1);
					//		for (int j = 0; j < numClients; j++)
					//		{
					//			//printf("%d", clients[j]);
					//			send(clients[j], sendBuf, strlen(sendBuf), 0);
					//		}
					//	}
					//	else
					//	{
					//		//printf("This");
					//		sprintf(sendBuf, "%s %s", id, buf + strlen(targetID) + 1);
					//		for (int k = 0; k < numClients; k++)
					//		{
					//			//printf("%d", clients[k]);
					//			if (strcmp(ids[k], targetID) == 0)
					//			{
					//				//printf("%d", clients[k]);
					//				send(clients[k], sendBuf, strlen(sendBuf), 0);
					//				printf("%d", clients[k]);
					//			}
					//		}
					//	}
					//}

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
