#include<stdio.h>
#include<memory.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<stdlib.h>
int serv_id;
int client_fd1,client_fd2;
char buff1[30],buff2[30];
int pid1,pid2;
int max_fd;
int i;
int sd;
int max_sock=2;
int client_socks[2];
void main()
{
fd_set set;
struct sockaddr_in server,client;
int serv_len=sizeof(server);

serv_id=socket(AF_INET,SOCK_STREAM,0);

memset(&server,0,sizeof(server));

server.sin_family=AF_INET;
server.sin_addr.s_addr=inet_addr("192.168.1.102");
server.sin_port=htons(5000);

printf("ser pid %d\n",getpid());
bind(serv_id,(struct sockaddr*)&server,sizeof(server));
listen(serv_id,1);
printf("server is waiting for a client connection.....\n");



while(1)
{
printf("while(1)===================================\n");
FD_ZERO(&set);
FD_SET(serv_id,&set);printf("set serv_id %d\n",serv_id);
max_fd=serv_id;

for(i=0;i<=max_sock;i++)
{
	if(client_socks[i]>0)
	{
		FD_SET(client_socks[i],&set);printf("setting clients %d in %d pos\n",client_socks[i],i);
	}
	if(sd>max_fd)
	{
		max_fd=sd;printf("maxfd became: %d\n",max_fd);
	}

}	

select(max_fd+1,&set,0,0,0);
printf("--------select---success----------\n");
if(FD_ISSET(serv_id,&set))
{

	client_fd1=accept(serv_id,(struct sockaddr*)&client,&serv_len);//goes PEND state-if server got client connection and go to ready stage,
	printf("client1: %d is connected\n",client_fd1);
	
		//printf("Waiting for client1 response\n");
		
		for(i=0;i<=max_sock;i++)
		{
			if(client_socks[i]==0)
			{
				printf("filled client_fd %d at %d position\n",client_fd1,i);
				client_socks[i]=client_fd1;
				FD_SET(client_socks[i],&set);break;
			}
		}	
}
for(i=0;i<=max_sock;i++)
{
	sd=client_socks[i];
	if(sd>0)
	{
	if(FD_ISSET(sd,&set))
	{
		printf("FD_ISSET sd %d =true :Waiting for client %d response\n",sd,sd);
		recv(sd,&buff1,30,0);
		if(strncmp(buff1,"EXIT",4)==0){printf("server is disconnected\n");exit(0);}
		printf("%s\n",buff1);

		printf("Enter message for client %d:\n",sd);
		fgets(buff1,30,stdin);
		send(sd,&buff1,30,0);
		//break;
	}
	printf("FD_ISSET sd %d =false :\n",sd);
	}
//printf("checking client_fd %d stored in array\n",client_socks[i]);	
}

}
}




