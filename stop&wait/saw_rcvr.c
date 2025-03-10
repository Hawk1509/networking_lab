#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct pkt
{
	int seq;
	int ack;
	char data[100];
}seqpkt,ackpkt;

void main()
{
    	int sock_desc,d,seqn=0;
    	socklen_t len;
    	struct sockaddr_in server, client;
    
    	sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    	if (sock_desc == -1) {
        	perror("socket creation failed\n");
        	exit(1);
    	}

    	server.sin_family = AF_INET;
   	server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
   	server.sin_port = htons(3003);       
	

	
   	if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) < 0) 
   	{
		perror("bind failed\n");
		exit(1);
    	}
	while(1)
	{
	    	len = sizeof(client);
	    	if (recvfrom(sock_desc, &seqpkt, sizeof(seqpkt), 0, (struct sockaddr*)&client, &len) > 0) 
	    	{
			d=rand()%10;
			if (d<2)
			{
				printf("Dropped packet:%d\n",seqpkt.seq);	
			}
			else if (d<5)
			{
				printf("Recieved Packet :%d\n Data:%s\n",seqpkt.seq,seqpkt.data);
				printf("Dropped Acknowledgement:%d\n",seqpkt.seq);
				seqn=(seqn+1)%2;
			}
			else
			{
				if(seqpkt.seq==seqn)
				{
					printf("Duplicate Packet:%d\n",seqpkt.seq);
				}
				else
				{
					printf("Recieved Packet:%d\n Data:%s\n",seqpkt.seq,seqpkt.data);
					seqn=(seqn+1)%2;
					printf("Acknowledgement Sent:%d\n",seqn);
					
				}
				len = sizeof(client);
				ackpkt.ack=seqn;
			    	if ( sendto(sock_desc, &ackpkt, sizeof(ackpkt), 0, (struct sockaddr*)&client, len) < 0) 
			    	{
					printf("Error in sending\n");
				}
				
				
			}
	    	}
	  }
	
}
