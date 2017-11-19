/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>

#define BYTE_MAX 1024
#define PORT 5000

using namespace std;

void readProducts(char *fname, sock) {

}

int main() {
    int lsock, sock, bytes_recieved, True = 1;
    char send_data[BYTE_MAX], recv_data[BYTE_MAX];

    struct sockaddr_in server_addr, client_addr;
    int sin_size;

    char *pfile = "products.txt";
    char *ofile = "orders.txt";

    /*[1] Create listen socket*/
    if ((lsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[Error] Fail to create socket\n");
        exit(1);
    }

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &True, sizeof(int)) == -1) {
        perror("[Error] Fail to set sock option");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(lsock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr))
        == -1) {
        perror("[Error] Fail to bind");
        exit(1);
    }

    if (listen(lsock, 5) == -1) {
        perror("[Error] Fail to Listen");
        exit(1);
    }

    cout << "TCPServer Waiting for client on port" << PORT << endl;
    fflush(stdout);


    while (1) {

        sin_size = sizeof(struct sockaddr_in);
        sock = accept(lsock, (struct sockaddr *) &client_addr, &sin_size);

        cout << "I got a connection from (" << inet_ntoa(client_addr.sin_addr)
             << " , " << ntohs(client_addr.sin_port) << endl;

        while (1) {
            bytes_recieved = recv(connected, recv_data, BYTE_MAX, 0);
            recv_data[bytes_recieved] = '\0';

            cout << "[Receive] " << recv_data << endl;
            fflush(stdout);
            if(recv_data == '5')
                break;

        }
    }

    close(lsock);
    close(sock);
    return 0;
}
