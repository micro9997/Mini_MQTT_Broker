
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

#include <stdlib.h>

struct PacketNode {
    uint8_t packetType;
    char topicName[10];
    uint8_t data;
};

struct NodeSubClents {
    uint8_t fd;
    struct NodeSubClents *next;
};

struct NodeTopic {
    char topic[10];
    struct NodeSubClents *headSubNode;
    struct NodeTopic *next;
};

struct NodeTopic *headTopicNode = NULL;

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     5000
#define SERVER_LENGTH   16
#define SERVER_BUFF     64

int8_t sub(struct PacketNode *data, int data_sock_fd) {
    if(headTopicNode == NULL) {
        struct NodeTopic *newNode = malloc(sizeof(struct NodeTopic));
        strcpy(newNode->topic, data->topicName);
        newNode->headSubNode = malloc(sizeof(struct NodeSubClents));
        newNode->headSubNode->fd = data_sock_fd;
        newNode->headSubNode->next = NULL;
        newNode->next = NULL;
        headTopicNode = newNode;
        return 0;
    } else {
        if(strcmp(headTopicNode->topic, data->topicName) == 0) {
            if(headTopicNode->headSubNode == NULL) {
                struct NodeSubClents *newNode = malloc(sizeof(struct NodeSubClents));
                newNode->fd = data_sock_fd;
                newNode->next = NULL;
                headTopicNode->headSubNode = newNode;
                return 0;
            } else {
                if(headTopicNode->headSubNode->fd == data_sock_fd) {
                    return 0;
                } else {
                    struct NodeSubClents *temp = headTopicNode->headSubNode;
                    while(temp->next != NULL) {
                        if(temp->next->fd == data_sock_fd) {
                            return 0;
                        }
                        temp = temp->next;
                    }
                    struct NodeSubClents *newNode = malloc(sizeof(struct NodeSubClents));
                    newNode->fd = data_sock_fd;
                    newNode->next = NULL;
                    temp->next = newNode;
                    return 0;
                }
            }
        } else {
            struct NodeTopic *temp = headTopicNode;
            while(temp->next != NULL) {
                if(strcmp(temp->next->topic, data->topicName) == 0) {
                    if(temp->next->headSubNode == NULL) {
                        struct NodeSubClents *newNode = malloc(sizeof(struct NodeSubClents));
                        newNode->fd = data_sock_fd;
                        newNode->next = NULL;
                        temp->next->headSubNode = newNode;
                        return 0;
                    } else {
                        if(temp->next->headSubNode->fd == data_sock_fd) {
                            printf("Uper Came\n");
                            return 0;
                        } else {
                            printf("Came\n");
                            struct NodeSubClents *tempp = temp->next->headSubNode;
                            while(tempp->next != NULL) {
                                if(tempp->next->fd == data_sock_fd) {
                                    return 0;
                                }
                                tempp = tempp->next;
                            }
                            struct NodeSubClents *newNode = malloc(sizeof(struct NodeSubClents));
                            newNode->fd = data_sock_fd;
                            newNode->next = NULL;
                            tempp->next = newNode;
                            return 0;
                        }
                    }
                }
                temp = temp->next;
            }
            struct NodeTopic *newNode = malloc(sizeof(struct NodeTopic));
            strcpy(newNode->topic, data->topicName);
            newNode->headSubNode = malloc(sizeof(struct NodeSubClents));
            newNode->headSubNode->fd = data_sock_fd;
            newNode->headSubNode->next = NULL;
            newNode->next = NULL;
            temp->next = newNode;
            return 0;
        }
    }

    return 0;
}

void print() {
    struct NodeTopic *temp = headTopicNode;
    while(temp != NULL) {
        printf("--> %s ", temp->topic);
        struct NodeSubClents *tempp = temp->headSubNode;
        if(tempp != NULL) {
            printf("[");
            while(tempp != NULL) {
                printf("%d, ", tempp->fd);
                tempp = tempp->next;
            }
            printf("] ");
        }
        temp = temp->next;
    }
    printf("\n");
}

void *handleMess(void *data_sock_fdd) {
    while(1) {
        int data_sock_fd = (long)data_sock_fdd;
        char serv_buffer[SERVER_BUFF];
        memset(serv_buffer, 0, sizeof(serv_buffer));

        struct PacketNode *data = malloc(sizeof(struct PacketNode));
        if(data == NULL) {
            print("Error in malloc\n");
            break;
        }

        // Try to get some actual data from client
        int byteCount;
        if((byteCount = recv(data_sock_fd, (void *)data, sizeof(struct PacketNode), 0)) != 0) {
            // printf("%d bytes of data are received | FD: %d\n", byteCount, data_sock_fd);
        } else {
            // printf("Error: data receive!\n");
            printf("Client disconnteced! | FR: %d\n", data_sock_fd);
            close(data_sock_fd);
            break;
        }

        // Print the data received from client
        // printf("Here is the client data | FD: %d | : %s\n", data_sock_fd, serv_buffer);
        // printf("%s\n", data->topicName);
        if(data->packetType == 0) {
            sub(data, data_sock_fd);
        }

        print();
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
