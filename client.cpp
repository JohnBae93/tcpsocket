/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
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
    int sock, bytes_recieved;
    char recv_data[BYTE_MAX];
    struct hostent *host;
    struct sockaddr_in server_addr;

    char* fname = "task.txt";
    string line;

    /* [1] Open input file */
    ifstream infile(fname);
    if (infile.is_open()) {
        cout << "[Clear] Success to open " << fname << endl;

        /* [2] Create socket*/
        host = gethostbyname("127.0.0.1");

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("[Error] Fail to create socket\n");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr = *((struct in_addr *) host->h_addr);
        bzero(&(server_addr.sin_zero), 8);

        if (connect(sock, (struct sockaddr *) &server_addr,
                    sizeof(struct sockaddr)) == -1) {
            perror("[Error] Fail to connect server\n");
            exit(1);
        }

        /* [3] Read file line by line*/
        while (getline(infile, line)) {
            cout << "[Clear] Read and send <" << line << ">" << endl;
            send(sock, line.c_str(), line.length(), 0);

            /* */
            cout << "[Receive]" << endl;
            while (bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0) > 0) {
                recv_data[bytes_recieved] = '\0';
                cout << recv_data << endl;
            }
        }


    } else {
        cout << "[Error] Fail to open " << fname << " in directory" << endl;
        exit(1);
    }

    infile.close();
    close(sock);
    return 0;
}
