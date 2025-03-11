#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
void main()
{
	int sock_desc=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=2005;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	int k=connect(sock_desc,(struct sockaddr*)&server,sizeof(server));
	char buf[100],revcbuf[100];
	fgets(buf,100,stdin);
	printf("\nEnter message to send to client: ");
	k=send(sock_desc,buf,100,0);
	int bytes_received = recv(sock_desc, revcbuf, sizeof(revcbuf), 0);
	printf("Message from server: %s\n", revcbuf);
}