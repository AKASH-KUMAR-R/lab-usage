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
	char buff[20],c[20];
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket failed");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&server.sin_addr);
	if(connect(sd,(struct sockaddr*)&server,sizeof(server))<0){
		perror("connect failed");
		exit(1);
	}
	printf("Enter file name:");
	scanf("%s",&buff);
	write(sd,buff,sizeof(buff));
	int n;
	while((n=read(sd,c,sizeof(c)))>0) {
		printf("%s",c);
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
	int sd,cli_len,data;
	char buff[20],c[20];
	if((sd=socket(AF_INET,SOCK_STREAM,0))<0){
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
	listen(sd,5);
	if((data=accept(sd,(struct sockaddr*)&cli,&cli_len))<0){
		perror("accept failed");
		exit(1);
	}
	read(data,buff,sizeof(buff));
	int fp;
	if((fp=open(buff,O_RDONLY))<0){
		perror("error opening file");
		exit(1);
	}
	int n;
	while((n=read(fp,c,sizeof(c)))>0) {
		write(data,c,sizeof(c));
	}
	close(data);
	close(sd);
}
