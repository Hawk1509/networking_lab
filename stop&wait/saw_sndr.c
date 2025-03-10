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
}pckt,rcvpkt;
int main() 
{
    char buf[100];
    int k,recvl;
    int sock_desc;
    struct timeval timeout={2,0};
    struct sockaddr_in server, client;
    socklen_t len = sizeof(server);

    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_desc == -1) 
    {
        perror("Error in socket creation\n");
        exit(1);
    }

    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_port = htons(3003); 
    
     
    pckt.seq=0;
    while(1)
    {
	    	printf("Enter data to be sent: ");
	    	fgets(pckt.data, sizeof(pckt.data), stdin); 
	    	printf("Sending %d %s\n",pckt.seq,pckt.data);
	    	k = sendto(sock_desc, &pckt, sizeof(pckt), 0, (struct sockaddr *)&server, len);
	    	if (k == -1) 
	    	{
			perror("Error in sending data\n");
			exit(1);
	    	}
	    	else
	    	{
	    		pckt.seq=(pckt.seq+1)%2;
		}
    
	    while(1)
	    {
	    	setsockopt(sock_desc,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
	    	
	    	recvl=recvfrom(sock_desc, &rcvpkt, sizeof(rcvpkt), 0, (struct sockaddr *)&server, &len);
	    	
	    	if(recvl<0)
	    	{
	    		printf("No acknowledgement:Retransmitting:%d\n",pckt.seq-1);
			pckt.seq=(pckt.seq+1)%2;
			k = sendto(sock_desc, &pckt, sizeof(pckt), 0, (struct sockaddr *)&server, len);
		    	if (k == -1) 
		    	{
				perror("Error in sending data\n");
				exit(1);
		    	}
		    	else
		    	{
		    		pckt.seq=(pckt.seq+1)%2;
			}
		
	    		
		}
		else
		{
			
			if(rcvpkt.ack==pckt.seq-1);
	    		printf("Acknowledgement Recieved:%d\n\n\n",rcvpkt.ack);
	    		break;
		}
		
	    }
	}
	

    close(sock_desc);
    return 0;
}
