
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>

struct PacketNode {
    uint8_t packetType;
    char topicName[25];
    int data;
};

#define SERVER_IP_ADDRESS	"127.0.0.1"
#define SERVER_PORT			5000

void *receiveData(void *fd) {
    while(1) {
        int data_sock_fd = (long)fd;

        struct PacketNode *data = malloc(sizeof(struct PacketNode));
        if(data == NULL) {
            printf("Error in malloc\n");
            break;
        }

        // Try to get some actual data from client
        int byteCount;
        if((byteCount = recv(data_sock_fd, (void *)data, sizeof(struct PacketNode), 0)) != 0) {
            printf("\n\nData received from Broker\n");
            printf("Topic: %s\n", data->topicName);
            printf("Data: %d\n\n", data->data);
            free(data);
        } else {
            printf("\n\nBroker Down!\n");
            close(data_sock_fd);
            free(data);
            break;
        }
    }
}

int main() {
    // Create a client socket
    int sock_fd;
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create socket\n");
        return 1;
    } else {
        printf("Socket successfully created\n");
    }

    // Populate it with server's address details
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT); 
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    // Connect
    if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    perror("Eerror");
        close(sock_fd);
        exit(1);
    } else {
        printf("Connected successfully to the server\n");
    }

    // Thread for data reception
    pthread_t tID;
    pthread_create(&tID, NULL, receiveData, (void *)(long)sock_fd);

    while(1) {
        // Prepare the data
        struct PacketNode *data = malloc(sizeof(struct PacketNode));
        printf("\nPlese enter the datas\n");
        printf("Packet type: ");
        scanf("%hhd", &data->packetType);
        printf("Packet topic: ");
        scanf("%s", data->topicName);
        if(data->packetType == 1) {
            printf("Packet data: ");
            scanf("%d", &data->data);
        } else {
            data->data = 0;
        }

        // Send the message to broker
        int c_size;
        if(c_size = send(sock_fd, (void *)data, sizeof(struct PacketNode), 0) > 0) {
            if(data->packetType == 0) {
                printf("The data was subscribed successfully\n");
            } else if(data->packetType == 1) {
                printf("The data was published successfully\n");
            }
        } else {
            printf("Error: Message send!\n");
        }

        free(data);

        char c;
        scanf("%c", &c);
    }

    close(sock_fd);
}
