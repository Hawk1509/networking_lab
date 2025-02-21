#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
void main()
{
	int sock_desc,client_sock_desc;
	char buf[100];
	sock_desc=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server_addr,client_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=2005;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr));
	socklen_t client_len=sizeof(client_addr);
	recvfrom(sock_desc,buf,100,0,(struct sockaddr*)&client_addr,&client_len);
	printf("Connection established");
	int bytes_received=recv(client_sock_desc,buf,100,0);
	printf("\nMessage from client: %s\n",buf);
}

/* OUTPUT
developerb1@ccfl3-pc39:~/jopa$ gcc s_udp.c
developerb1@ccfl3-pc39:~/jopa$ ./a.out
Connection established
Message from client: hello there
*/

