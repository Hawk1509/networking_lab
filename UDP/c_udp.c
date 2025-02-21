#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
void main()
{
	int sock_desc,k;
	char buf[100];
	sock_desc=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=2005;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	printf("Enter the msg to be sent:");
	fgets(buf,100,stdin);
	sendto(sock_desc,buf,100,0,(struct sockaddr*)&server,sizeof(server));
}

/*OUTPUT
developerb1@ccfl3-pc39:~/jopa$ gcc c_udp.c
developerb1@ccfl3-pc39:~/jopa$ ./a.out
Enter the msg to be sent:hello there
*/
