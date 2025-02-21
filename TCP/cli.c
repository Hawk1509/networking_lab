#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
void main()
{
int sock_desc,k;
char buf[100];
sock_desc = socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in server;
          server.sin_family=AF_INET;
          server.sin_port=2005;
  	server.sin_addr.s_addr=inet_addr("127.0.0.1");     	
k=connect(sock_desc,(struct sockaddr*)&server,sizeof(server));
printf("Enter message to be sent: ");
   	fgets(buf,100,stdin);
   	k=send(sock_desc,buf,100,0);

}
