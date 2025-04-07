

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>


#define PORT 3000
#define MAX_ROW 10
#define MAX_COL 10


int main() {

    int clientDescriptor;
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);

    int size[2][2];
    int matrix1[MAX_ROW][MAX_COL], matrix2[MAX_ROW][MAX_COL], result[MAX_ROW][MAX_COL];


    if ( (clientDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("\nFailed to create socket");
        exit(1);

    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if ( inet_pton( AF_INET, "127.0.0.1", &serverAddr.sin_addr) < 0) {

        printf("\nFailed to convert ip");
        exit(1);

    } 

    for (int i = 0;i < 2;i++) {

        printf("\nEnter the size of matrix%d\n", i);
        scanf("%d", &size[i][0]);
        scanf("%d", &size[i][1]);

    }

    printf("\nEnter matrix1\n");

    for (int i = 0;i < size[0][0];i++) {

        for (int j = 0;j < size[0][1];j++) {
            scanf("%d", &matrix1[i][j]);
        }

    }

    printf("\nEnter matrix2\n");

    for (int i = 0;i < size[1][0];i++) {

        for (int j = 0;j < size[1][1];j++) {
            scanf("%d", &matrix2[i][j]);
        }

    }

    
    if ( connect( clientDescriptor, (struct sockaddr*)&serverAddr, serverAddrLen) < 0) {

        printf("\nFailed to connect to server");
        exit(1);

    }

    send(clientDescriptor, &size, sizeof(size), 0);

    send(clientDescriptor, &matrix1, sizeof(matrix1), 0);

    send(clientDescriptor, &matrix2, sizeof(matrix2), 0);

    int resultSize[1][2];

    read(clientDescriptor, &resultSize, sizeof(resultSize));
    
    if ( (resultSize[0][0] != -1) && (resultSize[0][1] != -1) ) {
        read(clientDescriptor, &result, sizeof(result));

        for (int i = 0;i < resultSize[0][0];i++) {

            for (int j = 0;j < resultSize[0][1];j++) {

                printf("\t%d", result[i][j]);

            }

            printf("\n");
        }

    } else {
        printf("\nMatrix multiplication is not possible!!!!");
    }

    close(clientDescriptor);

    return 0;
    
}

server



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>


#define PORT 3000
#define MAX_ROW 10
#define MAX_COL 10


int main() {

    int serverDescriptor, clientDescriptor;
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);

    int size[2][2];
    int matrix1[MAX_ROW][MAX_COL], matrix2[MAX_ROW][MAX_COL], result[MAX_ROW][MAX_COL] = {0};


    if ( (serverDescriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        printf("\nFailed to create socket");
        exit(1);

    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;



    if ( bind( serverDescriptor, (struct sockaddr*)&serverAddr, serverAddrLen) < 0) {

        printf("\nFailed to bind to server");
        exit(1);

    }

    listen(serverDescriptor, 5);

    if ( (clientDescriptor = accept( serverDescriptor, (struct sockaddr*)&serverAddr, &serverAddrLen)) < 0) {

        printf("\nFailed to accept incoming connections");
        exit(1);

    }

    read(clientDescriptor, &size, sizeof(size));

    for (int i = 0;i < 2;i++) {

        for (int j = 0;j < 2;j++) {

            printf("\t%d", size[i][j]);

        }

        printf("\n");
    }

    read(clientDescriptor, &matrix1, sizeof(matrix1));

    for (int i = 0;i < size[0][0];i++) {

        for (int j = 0;j < size[0][1];j++) {

            printf("\t%d", matrix1[i][j]);

        }

        printf("\n");
    }

    read(clientDescriptor, &matrix2, sizeof(matrix2));

    for (int i = 0;i < size[1][0];i++) {

        for (int j = 0;j < size[1][1];j++) {

            printf("\t%d", matrix2[i][j]);

        }

        printf("\n");
    }


    int resultSize[1][2];

    if (size[0][1] == size[1][0]) {

        for (int i = 0; i < size[0][0];i++) {

            for (int j = 0;j < size[0][1];j++) {

                for (int k = 0;k < size[1][0];k++) {
    
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
    
                }
            }
        }

        resultSize[0][0] = size[0][0];
        resultSize[0][1] = size[1][1];

        send( clientDescriptor, &resultSize, sizeof(resultSize), 0);
        send( clientDescriptor, &result, sizeof(result), 0);

    } else {

        resultSize[0][0] = -1;
        resultSize[0][1] = -1;
        
        send( clientDescriptor, &resultSize, sizeof(resultSize), 0);

    }

    close(clientDescriptor);
    close(serverDescriptor);

    return 0;
}


