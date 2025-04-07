#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[]) {
	struct sockaddr_in server;
	int sd;
	char buff[20];
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket failed");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&server.sin_addr);
	
	
	while(1) {
		bzero(buff,sizeof(buff));
		fgets(buff,sizeof(buff),stdin);
		
		
		sendto(sd,buff,sizeof(buff),0,(struct sockaddr*)&server,sizeof(server));
		
		
	}
	
	close(sd);
}


server

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[]) {
	struct sockaddr_in server,cli;
	int sd,cli_len=sizeof(cli);
	char buff[20];
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket failed");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sd,(struct sockaddr*)&server,sizeof(server))<0){
		perror("bind failed");
		exit(1);
	}
	
	
	int n;
	while(1) {
		bzero(buff,sizeof(buff));
		n=recvfrom(sd,buff,sizeof(buff),0,(struct sockaddr*)&cli,&cli_len);
		if(n<=0){
			printf("error reading");
			break;
		}
		
		
		printf("%s",buff);
		
	}
	close(sd);
}
