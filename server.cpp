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
#include <vector>

#define BYTE_MAX 1024
#define PORT 5000

using namespace std;

int main() {
    int lsock, sock, bytes_recieved, True = 1;
    char recv_data[BYTE_MAX];
    string send_data;
    struct sockaddr_in server_addr, client_addr;
    unsigned sin_size;

    string *pfname = "products.txt";
    string *ofname = "orders.txt";

    int pos = 0, pre_pos = 0;
    string line; // get line from input file
    vector <vector<string>> orders;
    string order; // make order string
    string cancle; // cancle string
    string order_id;
    string product_id;
    string cancel_id;

    string result; // command result to send

    /*[1] Create listen socket*/
    if ((lsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[Error] Fail to create socket");
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

    cout << "TCPServer Waiting for client on port: " << PORT << endl;
    fflush(stdout);

    /* [2] Infinite loop for client */
    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        sock = accept(lsock, (struct sockaddr *) &client_addr, &sin_size);

        cout << "I got a connection from (" << inet_ntoa(client_addr.sin_addr)
             << " , " << ntohs(client_addr.sin_port) << ")" << endl;

        /* [3] Communicate */
        while (1) {
            bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
            recv_data[bytes_recieved] = '\0';

            cout << "[Receive] " << recv_data << endl;

            /* [3-1] List of Products */
            if (recv_data[0] == '1') {
                ifstream infile(pfname);

                if (infile.is_open()) {
                    cout << "[Clear] Success to open " << pfname << endl;

                    send_data = ""; // init
                    while (getline(infile, line)) {
                        send_data.append(line);
                        send_data.append("\n");
                    }
                    send(sock, send_data.c_str(), send_data.length(), 0);
                    cout << "[Clear] Read and send " << endl;
                    cout << send_data << endl;
                    infile.close();
                } else {
                    result = "Server fail to find product file\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Fail to open " << pfname << " in directory" << endl;
                    break;
                }
            }

            /* [3-2] List of Orders */
            if (recv_data[0] == '2') {
                ifstream infile(ofname);

                if (infile.is_open()) {
                    cout << "[Clear] Success to open " << ofname << endl;

                    send_data = ""; // init
                    while (getline(infile, line)) {
                        send_data.append(line);
                        send_data.append("\n");
                    }
                    send(sock, send_data.c_str(), send_data.length(), 0);
                    cout << "[Clear] Read and send " << endl;
                    cout << send_data << endl;
                    infile.close();
                } else {
                    result = "Server fail to find order file\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                    break;
                }
            }

            /* [3-3] Make an order */
            if (recv_data[0] == '3') {
                send(sock, "OK\n", 2, 0);
                bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
                recv_data[bytes_recieved] = '\0';

                cout << "[Receive] " << recv_data << endl;

                order = recv_data;
                pre_pose = 0;
                pos = order.find_first_of(" ", pre_pose);
                if (order.substr(pre_pose, pos).compare("Make_An_Order") == 0) {
                    pre_pose = pos + 1;

                    ofstream outfile;
                    outfile.open(ofname, ofstream::out | ofstream::app);

                    if (outfile.is_open()) {
                        cout << "[Clear] Success to open " << ofname << endl;

                        pos = order.find_first_of(" ", pre_pose);
                        order_id = order.substr(pre_pose, pos - pre_pose);
                        pre_pose = pos + 1;

                        product_id = order.substr(pre_pose, pos - pre_pose);
                        outfile << product_id << " " << order_id << '\n';
                        outfile.close();

                        result = "Your order has been made!\n";
                        send(sock, result.c_str(), result.length(), 0);
                    } else {
                        result = "Server fail to find order file\n";
                        send(sock, result.c_str(), result.length(), 0);
                        cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                        break;
                    }
                } else {
                    string result = "Your order are not correct\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Make order are not correct" << endl;
                }
            }

            /* [3-4] Cancel an order */
            if (recv_data[0] == '4') {
                send(sock, "ok", 2, 0);
                bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
                recv_data[bytes_recieved] = '\0';

                cout << "[Receive] " << recv_data << endl;

                cancel = recv_data;
                pre_pose = 0;
                int isFind = 0;
                pos = cancel.find_first_of(" ", pre_pose);
                if (cancel.substr(pre_pose, pos).compare("Cancel_Order") == 0) {
                    pre_pose = pos + 1;

                    vector <string> s;
                    s.resize(2);

                    ifstream infile(ofname);
                    if (infile.is_open()) {
                        cout << "[Clear] Success to open " << ofname << endl;
                        pos = cancel.find_first_of(" ", pre_pose);
                        cancel_id = cancel.substr(pre_pose, pos - pre_pose);

                        while (getline(infile, line)) {
                            pos = line.find_first_of(" ", 0);
                            pre_pose = pos + 1;
                            product_id = line.substr(0, pos);
                            order_id = line.substr(pre_pose);
                            if (order_id == cancel_id) {
                                isFind = 1;
                            } else {
                                s[0] = product_id;
                                s[1] = order_id;
                                orders.push_back(s);
                            }
                        }
                        infile.close();
                        /* [3-4-1] If canceled order exist*/
                        if (isFind == 0) {
                            string result = "Your ";
                            result.append(cancel_id);
                            result.append(" does not exist!!!\n");
                            result.append("Pleas try again\n");
                            send(sock, result.c_str(), result.length(), 0);
                        } else { // not exist
                            ofstream outfile;
                            outfile.open(ofname);
                            for (int i = 0; i < orders.size(); ++i) {
                                s = orders[i];
                                outfile << s[0] << " " << s[1] << '\n';
                            }
                            outfile.close();

                            result = "Your order has been been cancel!\n";
                            send(sock, result.c_str(), result.length(), 0);
                        }
                    } else {
                        result = "Server fail to find order file\n";
                        send(sock, result.c_str(), result.length(), 0);
                        cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                        break;
                    }
                } else {
                    string result = "Your cancel are not correct\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Cancel order are not correct" << endl;
                }
            }

            if (recv_data[0] == '5') {
                cout << "[Exit] Terminate connect with client" << endl;
                send(sock, "EXIT\n", 5, 0);
                break;
            }

        }
        close(sock);
    }
    close(lsock);

    return 0;
}
