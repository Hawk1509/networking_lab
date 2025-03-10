//TCP 

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
void main()
{
	int sock_desc,client_sock_desc;
	char buf[100];
	sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=2005;
		server_addr.sin_addr.s_addr= INADDR_ANY;
	bind(sock_desc,(struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(sock_desc, 3);
	struct sockaddr_in client_addr;
	socklen_t client_len;
	client_len = sizeof(client_addr);
	client_sock_desc = accept(sock_desc, (struct sockaddr*)&client_addr, &client_len);
	//receive data from the client
	printf("Connection established");
	int bytes_received =recv(client_sock_desc, buf,100,0);

	printf("Message from client: %s\n", buf);
}
