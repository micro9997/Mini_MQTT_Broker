
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

struct PacketNode {
    uint8_t packetType;
    char topicName[25];
    uint8_t data;
};

struct NodeSubClents {
    uint8_t fd;
    struct SubNode *next;
};

struct NodeTopic {
    char topic[10];
    struct NodeSubClents *headSubNode;
    struct NodeTopic *next;
};

struct NodeTopic *headTopicNode;

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     5000
#define SERVER_LENGTH   16
#define SERVER_BUFF     64

void *handleMess(void *data_sock_fdd) {
    while(1) {
        int data_sock_fd = (long)data_sock_fdd;
        char serv_buffer[SERVER_BUFF];
        memset(serv_buffer, 0, sizeof(serv_buffer));

        // Try to get some actual data from client
        int byteCount;
        if((byteCount = recv(data_sock_fd, (void *)serv_buffer, SERVER_BUFF, 0)) != 0) {
            // printf("%d bytes of data are received | FD: %d\n", byteCount, data_sock_fd);
        } else {
            // printf("Error: data receive!\n");
            printf("Client disconnteced! | FR: %d\n", data_sock_fd);
            close(data_sock_fd);
            break;
        }

        // Print the data received from client
        printf("Here is the client data | FD: %d | : %s\n", data_sock_fd, serv_buffer);
    }
}

int main() {
    // Create a TCP socket
    int sock_fd;
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create socket\n");
        return 1;
    } else {
        printf("Socket successfully created\n");
    }

    // Bind it to a particular IP address & port)
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    // Binding
    if(bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0) {
        printf("Bound successfully\n");
    } else {
        printf("Error binding!");
        return 1;
    }

    // listen for incoming connections
    if((listen(sock_fd, SERVER_LENGTH)) == 0) {
        printf("Broker started running . .\n");
    } else {
        printf("Error in running the broker!");
        return 1;
    }

    // Receive clients
    while(1) {
        int data_sock_fd = accept(sock_fd, (struct sockaddr*)NULL, NULL);
        if(data_sock_fd != -1) {
            printf("A client connected | FD: %d\n", data_sock_fd);
        } else {
            printf("Error in client connect!\n");
        }

        pthread_t tID;
        pthread_create(&tID, NULL, handleMess, (void *)(long)data_sock_fd);
    }

    // Close the sockets now
    close(sock_fd);
}
