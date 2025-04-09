
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>



#define PORT 3000
#define BUFFER_SIZE 1024

int main() {

    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE] = {0};

    if ( (socketDescriptor = socket( AF_INET, SOCK_STREAM, 0) ) < 0) {

        printf("\nFailed to create client socket");
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (inet_pton( AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {

        printf("\n Failed to convert ip address");
        exit(1);
    }

    if (connect( socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {

        printf("\nFailed to connect to the server");
        exit(1);

    }

    printf("\nEnter the message: ");
    scanf("%[^\n]", buffer);

    send( socketDescriptor, buffer, strlen(buffer), 0);
    printf("Message sended");


    read( socketDescriptor, buffer, BUFFER_SIZE);

    printf("\nReversed message : %s", buffer);


    close(socketDescriptor);

    return 0;

}


SERVER



#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define PORT 3000
#define BUFFER_SIZE 1024

int main() {

    int socketDescriptor, clientDescriptor;
    struct sockaddr_in serverAddress;
    socklen_t serverAddrLen = sizeof(serverAddress);

    char buffer[BUFFER_SIZE] = {0};


    if ((socketDescriptor = socket( AF_INET, SOCK_STREAM, 0)) <= 0) {

        printf("Failed to create socket");
        exit(1);

    }

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;


    if (bind( socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {

        printf("\nFailed to bind the socket");
        exit(1);

    }

    printf("\nServer is listening on port %d", PORT);
    
    if (listen( socketDescriptor, 5) < 0) {

        printf("\nFailed to listen for incomming connections");
        exit(1);

    }

    if ( (clientDescriptor = accept( socketDescriptor, (struct sockaddr*)&serverAddress, &serverAddrLen ) ) < 0) {

        printf("\nFailed to accept incomming connections");
        exit(1);

    }

    int bytesReaded = read( clientDescriptor, buffer, BUFFER_SIZE);
    printf("\nReaded string is: %s", buffer);

    char temp;
    int j;

    for (int i = 0, j = strlen(buffer) - 1;i <= j;i++, j--) {

        temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;

    }

    send( clientDescriptor, buffer, strlen(buffer), 0);

    close(socketDescriptor);
    close(clientDescriptor);

    return 0;

}
