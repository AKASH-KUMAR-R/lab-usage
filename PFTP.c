
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 3000


int main() {

    int sock_ctrl, sock_data, coveredLen, filePtr;
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);

    char fileName[BUFFER_SIZE], buffer[BUFFER_SIZE], command[BUFFER_SIZE];


    if ( (sock_ctrl = socket( AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("\nFailed to create control socket");
        exit(1);

    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if ( inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) < 0) {

        printf("\nFailed to convert ip");
        exit(1);

    }

    if ( connect( sock_ctrl, (struct sockaddr*)&serverAddr, serverAddrLen) < 0) {

        printf("\nFailed to connect to the server");
        exit(1);

    }

    while(1) {

        printf("\nOperations are\nget: accessing a file\nclose: closing the connection\n>>");

        if ( ( sock_data = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

            printf("\nFailed to create data socket");
            exit(1);

        }

        scanf("%s", command);

        send(sock_ctrl, command, strlen(command), 0);

        if (strcmp(command, "close") == 0) {
            close(sock_ctrl);
            close(sock_data);
            break;
        }


        printf("\nEnter filename: ");
        scanf("%s", fileName);

        if ( (filePtr = open( fileName, O_TRUNC, 0644) ) < 0) {
            printf("\nFailed to open write file");
            exit(1);
        }    

        send( sock_ctrl, fileName, strlen(fileName), 0);


        serverAddr.sin_port = htons(PORT + 1);

        if ( connect(sock_data, (struct sockaddr*)&serverAddr, serverAddrLen) < 0) {

            printf("\nFailed to connect for data");
            exit(1);

        }

        do {

            coveredLen = read(sock_data, buffer, strlen(buffer));

            printf("%s", buffer);
            write(filePtr, buffer, coveredLen);

        } while(coveredLen > 0);


        if (buffer[0] == '@') {

            printf("%s \n", buffer);

        }

    }

    return 0;

}


SERVER


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define BUFFER_SIZE 1024
#define PORT 3000

int main() {

    int listen_ctrl, listen_data, sock_ctrl, sock_data;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE], command[BUFFER_SIZE], name[BUFFER_SIZE];
    int isFileFound = 1, coveredLen;

    int filePtr;

    
    if ( (listen_ctrl = socket( AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("\nFailed to create listen socket");
        exit(1);

    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind( listen_ctrl, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {

        printf("\nFailed to bind listen socket");
        exit(1);

    }


    if ( (listen_data = socket( AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("\nFailed to create data socket");
        exit(1);

    }

    serverAddr.sin_port = htons(PORT + 1);

    if (bind( listen_data, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {

        printf("\nFailed to bind data socket");
        exit(1);

    }

    listen(listen_ctrl, 5);
    listen(listen_data, 5);

    while(1) {


        if ( (sock_ctrl = accept( listen_ctrl, (struct sockaddr*)&serverAddr, &clientAddrLen)) < 0) {

            printf("\nFailed to accept incoming control connection");
            exit(1);

        }

        serverAddr.sin_port = htons(PORT + 1);

        if (fork() == 0) {

            close(listen_ctrl);

            while(1) {

                read( sock_ctrl, command, BUFFER_SIZE);

                printf("\nFROM CLIENT: %s \n", command);

                if (strcmp(command, "close") == 0) {
                    break;
                }

                read(sock_ctrl, name, BUFFER_SIZE);

                if ( (filePtr = open(name, O_RDONLY) ) < 0 ) {

                    sprintf(buffer, "@FILE NOT FOUND PROCESS ID %d", getpid());
                    isFileFound = 0;
                    printf("\nRequested file not found");
                }

                if ( (sock_data = accept( listen_data, (struct sockaddr*)&serverAddr, &clientAddrLen)) < 0) {

                    printf("\nFailed to accept incoming data connections");
                    exit(1);

                }

                if (isFileFound) {

                    do {
                        coveredLen = read( filePtr, buffer, sizeof(buffer) );

                        send( sock_data, buffer, coveredLen, 0);

                    } while (coveredLen > 0);

                    close(filePtr);

                    sprintf(buffer, "File %s received from server with process id %d", name, getpid());

                    send(sock_data, buffer, strlen(buffer), 0);

                } else {

                    send( sock_data, buffer, BUFFER_SIZE, 0);
                    bzero(buffer, sizeof(buffer));

                }

                close(sock_data);

            }

            close(sock_data);
            close(sock_ctrl);
            exit(1);
        }

    }

    close(listen_ctrl);
    close(listen_data);

    return 0;
}

