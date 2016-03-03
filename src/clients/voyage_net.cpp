#include "networking/Client.hpp"
#include "networking/Server.hpp"
#include <Kbhit.h>

char *remoteIPAddress=0;
char randomData1[RANDOM_DATA_SIZE_1];
char randomData2[RANDOM_DATA_SIZE_2];
static const int NUM_CLIENTS=100;

int main(void)
{
	Client clients[NUM_CLIENTS];
	Server server;
// int clientIndex;
	int mode;
	printf("Connects many clients to a single server.\n");
	printf("Difficulty: Intermediate\n\n");
	printf("Run as (S)erver or (C)lient or (B)oth? ");
	char ch = getche();
	static char *defaultRemoteIP="127.0.0.1";
	char remoteIP[128];
	static char *localIP="127.0.0.1";
	if (ch=='s' || ch=='S')
	{
		mode=0;
	}
	else if (ch=='c' || ch=='c')
	{
		mode=1;
		remoteIPAddress=remoteIP;
	}
	else
	{
		mode=2;
		remoteIPAddress=localIP;
	}
	printf("\n");
	if (mode==1 || mode==2)
	{
		printf("Enter remote IP: ");
		Gets(remoteIP, sizeof(remoteIP));
		if (remoteIP[0]==0)
		{
			strcpy(remoteIP, defaultRemoteIP);
			printf("Using %s\n", defaultRemoteIP);
		}
	}
	unsigned i;
	randomData1[0]=(char) ID_USER_PACKET_ENUM;
	for (i=0; i < RANDOM_DATA_SIZE_1-1; i++)
		randomData1[i+1]=i;
	randomData2[0]=(char) ID_USER_PACKET_ENUM;
	for (i=0; i < RANDOM_DATA_SIZE_2-1; i++)
		randomData2[i+1]=i;
	if (mode==0 || mode==2)
	{
		server.Start();
		printf("Started server\n");
	}
	if (mode==1 || mode==2)
	{
		printf("Starting clients...\n");
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].Startup();
		printf("Started clients\n");
		printf("Connecting clients...\n");
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].Connect();
		printf("Done.\n");
	}
	RakNet::TimeMS endTime = RakNet::GetTimeMS()+60000*5;
	RakNet::TimeMS time = RakNet::GetTimeMS();
	while (time < endTime)
	{
		if (mode==0 || mode==2)
			server.Update(time);
		if (mode==1 || mode==2)
		{
			for (i=0; i < NUM_CLIENTS; i++)
				clients[i].Update(time);
		}
		if (kbhit())
		{
			char ch = getch();
			if (ch==' ')
			{
				FILE *fp;
				char text[2048];
				if (mode==0 || mode==2)
				{
					printf("Logging server statistics to ServerStats.txt\n");
					fp=fopen("ServerStats.txt","wt");
					for (i=0; i < NUM_CLIENTS; i++)
					{
						RakNetStatistics *rssSender;
						rssSender=server.peer->GetStatistics(server.peer->GetSystemAddressFromIndex(i));
						StatisticsToString(rssSender, text, 3);
						fprintf(fp,"==== System %i ====\n", i+1);
						fprintf(fp,"%s\n\n", text);
					}
					fclose(fp);
				}
				if (mode==1 || mode==2)
				{
					printf("Logging client statistics to ClientStats.txt\n");
					fp=fopen("ClientStats.txt","wt");
					for (i=0; i < NUM_CLIENTS; i++)
					{
						RakNetStatistics *rssSender;
						rssSender=clients[i].peer->GetStatistics(clients[i].peer->GetSystemAddressFromIndex(0));
						StatisticsToString(rssSender, text, 3);
						fprintf(fp,"==== Client %i ====\n", i+1);
						fprintf(fp,"%s\n\n", text);
					}
					fclose(fp);
				}
			}
			if (ch=='q' || ch==0)
				break;
		}
		time = RakNet::GetTimeMS();
		RakSleep(30);
	}
	if (mode==0 || mode==2)
		server.peer->Shutdown(0);
	if (mode==1 || mode==2)
		for (i=0; i < NUM_CLIENTS; i++)
			clients[i].peer->Shutdown(0);
	printf("Test completed");
	return 0;
}
