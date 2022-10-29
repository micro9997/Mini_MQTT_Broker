
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_IP_ADDRESS	"127.0.0.1"
#define SERVER_PORT			5000
#define CLIENT_BUFF_SIZE	64

int main() {
    int sock_fd, c_size;
    char client_buff[CLIENT_BUFF_SIZE];
    struct sockaddr_in serv_addr; 

    // Create a client socket
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error: Could not create socket\n");
        return 1;
    } else {
        printf("Socket successfully created\n");
    }

    // Binding with client socket is optional here

    // Populate it with server's address details
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT); 
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    perror("Eerror");
        close(sock_fd);
        exit(1);
    } else {
        printf("Connected successfully to the server\n");
    }

    while(1) {
        // Get the message from client
        printf("Enter the message you want to sent to server: \n");
        scanf("%[^\n]s", client_buff);

        // Send the message to server
        c_size = send(sock_fd, client_buff, strlen(client_buff) + 1, 0);

        if(c_size > 0) {
            printf("Message (%d bytes) sent to server successsfully, please check\n", c_size);
        } else {
            printf("Error: Message send!\n");
        }
        char c;
        scanf("%c", &c);
    }

    close(sock_fd);
}
