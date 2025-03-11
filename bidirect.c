#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
void main()
{
	int sock_desc=socket(AF_INET,SOCK_STREAM,0);
	if(sock_desc!=-1)
	{
		printf("socket created\n");
	}
	else
	printf("socket not created\n");
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=2005;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr));
	listen(sock_desc,3);
	struct sockaddr_in client_addr;
	socklen_t client_len=sizeof(client_addr);
	int client_sock_desc=accept(sock_desc,(struct sockaddr*)&client_addr,&client_len);
	char buf[50],sendbuf[50];
	int bytes_received = recv(client_sock_desc, buf, sizeof(buf), 0);	
	printf("Message from client:%s\n",buf);
	
	printf("\nEnter message to send to client: ");
	fgets(sendbuf,100,stdin);
	send(client_sock_desc, sendbuf, strlen(sendbuf) + 1, 0);

	
}