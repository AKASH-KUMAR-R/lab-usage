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
	char user[20],pass[20],msg[100];
	if((sd=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket failed");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&server.sin_addr);
	
	printf("Enter username:");
	scanf("%s",&user);
	sendto(sd,user,sizeof(user),0,(struct sockaddr*)&server,sizeof(server));
	
	printf("Enter password:");
	scanf("%s",&pass);
	sendto(sd,pass,sizeof(pass),0,(struct sockaddr*)&server,sizeof(server));
	recvfrom(sd,msg,sizeof(msg),0,NULL,NULL);
	
	printf("%s",msg);
	
	
	close(sd);
}

SERVER

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
	int sd,cli_len=sizeof(cli),data;
	char user[20],pass[20],msg[100];
	char username[20] = "admin";
	char password[20] = "admin@123";
	char msg1[20] = "login successful";
	char msg2[20] = "login failed";
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
	recvfrom(sd,user,sizeof(user),0,(struct sockaddr*)&cli,&cli_len);
	recvfrom(sd,pass,sizeof(pass),0,(struct sockaddr*)&cli,&cli_len);
	
	if((strcmp(user,username) && strcmp(pass,password)) == 0){
		strcpy(msg,msg1);
	}
	else{
		strcpy(msg,msg2);
	}
	sendto(sd,msg,sizeof(msg),0,(struct sockaddr*)&cli,cli_len);
	
	close(data);
	close(sd);
}
