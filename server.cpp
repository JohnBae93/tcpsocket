/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>

#define BYTE_MAX 1024
#define PORT 5000

using namespace std;

int main() {
    int lsock, sock, bytes_recieved, True = 1;
    char send_data[BYTE_MAX], recv_data[BYTE_MAX];

    struct sockaddr_in server_addr, client_addr;
    unsigned sin_size;

    char *pfile = "products.txt";
    char *ofile = "orders.txt";

    /*[1] Create listen socket*/
    if ((lsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[Error] Fail to create socket\n");
        exit(1);
    }

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &True, sizeof(int)) == -1) {
        perror("[Error] Fail to set sock option\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(lsock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr))
        == -1) {
        perror("[Error] Fail to bind\n");
        exit(1);
    }

    if (listen(lsock, 5) == -1) {
        perror("[Error] Fail to Listen\n");
        exit(1);
    }

    cout << "TCPServer Waiting for client on port" << PORT << endl;
    fflush(stdout);


    sin_size = sizeof(struct sockaddr_in);
    sock = accept(lsock, (struct sockaddr *) &client_addr, &sin_size);

    cout << "I got a connection from (" << inet_ntoa(client_addr.sin_addr)
         << " , " << ntohs(client_addr.sin_port) << ")" << endl;

    while (1) {
        bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
        recv_data[bytes_recieved] = '\0';

        cout << "[Receive] " << recv_data << endl;
        fflush(stdout);

//        if(recv_data[0] == '1') {
//            string line;
//            ifstream infile(pfile);
//
//            if (infile.is_open()) {
//                cout << "[Clear] Success to open " << pfile << endl;
//
//                while (getline(infile, line)) {
//                    line[line.length() - 1] = '\0';
//                    cout << "[Clear] Read and send <" << line << ">" << endl;
//                    send(sock, line.c_str(), line.length(), 0);
//                    recv(sock, recv_data, BYTE_MAX, 0);
//                }
//            }
//        }
        if(recv_data[0] == '5') {
            break;
        }

    }

    close(lsock);
    close(sock);
    return 0;
}
