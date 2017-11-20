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
    vector <vector<int> > orders;
    int lsock, sock, bytes_recieved, True = 1;
    char recv_data[BYTE_MAX];
    string send_data;
    struct sockaddr_in server_addr, client_addr;
    unsigned sin_size;

    char *pfname = "products.txt";
    char *ofname = "orders.txt";

    int order_id;
    int product_id;
    int cancel_id;
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

    cout << "TCPServer Waiting for client on port: " << PORT << endl;
    fflush(stdout);

    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        sock = accept(lsock, (struct sockaddr *) &client_addr, &sin_size);

        cout << "I got a connection from (" << inet_ntoa(client_addr.sin_addr)
             << " , " << ntohs(client_addr.sin_port) << ")" << endl;

        while (1) {
            bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
            recv_data[bytes_recieved] = '\0';

            cout << "[Receive] " << recv_data << endl;
            // fflush(stdout);

            if (recv_data[0] == '1') {
                string line;
                ifstream infile(pfname);

                if (infile.is_open()) {
                    cout << "[Clear] Success to open " << pfname << endl;

                    while (getline(infile, line)) {
                        send_data.append(line);
                        send_data.append("\n");
                    }
                    send(sock, send_data.c_str(), send_data.length(), 0);
                    cout << "[Clear] Read and send " << endl;
                    cout << send_data << endl;
                    infile.close();
                } else {
                    string result = "Some thing error in server\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Fail to open " << pfname << " in directory" << endl;
                    break;
                }

            }
            if (recv_data[0] == '2') {
                string line;
                ifstream infile(ofname);

                if (infile.is_open()) {
                    cout << "[Clear] Success to open " << ofname << endl;

                    while (getline(infile, line)) {
                        send_data.append(line);
                        send_data.append("\n");
                    }
                    send(sock, send_data.c_str(), send_data.length(), 0);
                    cout << "[Clear] Read and send " << endl;
                    cout << send_data << endl;
                    infile.close();
                } else {
                    string result = "Some thing error in server\n";
                    send(sock, result.c_str(), result.length(), 0);
                    cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                    break;
                }

            }
            if (recv_data[0] == '3') {
                send(sock, "ok", 2, 0);
                bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
                recv_data[bytes_recieved] = '\0';
                cout << "[Receive] " << recv_data << endl;

                string order = recv_data;
                int pos, pre_pose = 0;
                pos = order.find_first_of(" ", pre_pose);
                if (order.substr(pre_pose, pos).compare("Make_An_Order") == 0) {
                    pre_pose = pos + 1;

                    ofstream outfile;
                    outfile.open(ofname, ofstream::out | ofstream::app);
                    if (outfile.is_open()) {
                        cout << "[Clear] Success to open " << ofname << endl;
                        pos = order.find_first_of(" ", pre_pose);
                        order_id = atoi(order.substr(pre_pose, pos - pre_pose).c_str());
                        pre_pose = pos + 1;
                        product_id = atoi(order.substr(pre_pose, pos - pre_pose).c_str());
                        outfile << product_id << " " << order_id << '\n';

                        outfile.close();
                        string result = "Your order has been made!\n";
                        send(sock, result.c_str(), result.length(), 0);
                    } else {
                        string result = "Some thing error in server\n";
                        send(sock, result.c_str(), result.length(), 0);
                        cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                        break;
                    }

                } else {
                    cout << "[Error] Make order are not correct" << endl;

                    string result = "Your order are not correct\n";
                    send(sock, result.c_str(), result.length(), 0);
                }

            }
            if (recv_date == '4') {
                send(sock, "ok", 2, 0);
                bytes_recieved = recv(sock, recv_data, BYTE_MAX, 0);
                recv_data[bytes_recieved] = '\0';
                cout << "[Receive] " << recv_data << endl;

                string cancel = recv_data;
                int pos, pre_pose = 0;
                int isFind = 0;
                pos = cancel.find_first_of(" ", pre_pose);
                if (cancel.substr(pre_pose, pos).compare("Cancel_Order") == 0) {
                    pre_pose = pos + 1;

                    vector<int> s;
                    s.resize(2);
                    string line;
                    ifstream infile(ofname);
                    if (infile.is_open()) {
                        cout << "[Clear] Success to open " << ofname << endl;
                        pos = order.find_first_of(" ", pre_pose);
                        cancel_id = atoi(cancel.substr(pre_pose, pos - pre_pose).c_str());

                        while (getline(infile, line)) {
                            pose = line.find_first_of(" ", 0);
                            pre_pose = pos + 1;
                            product_id = atoi(line.substr(0, pos).c_str());
                            order_id = atoi(line.substr(pre_pose).c_str());
                            if (order_id == cancel_id) {
                                isFind = 1;
                            } else {
                                s[0] = product_id;
                                s[1] = order_id;
                                orders.push_back(s);
                            }
                        }
                        outfile.close();
                        if (isFind == 0) {
                            string result = "Your ";
                            result.append(cancel_id);
                            result.append(" does not exist!!!\n");
                            result.append("Pleas try again");
                            send(sock, result.c_str(), result.length(), 0);
                        } else {
                            ofstream outfile;
                            outfile.open(ofname);
                            for (int i = 0; i < orders.size(); ++i) {
                                order = orders[i];
                                outfile << order[0] << " " << order[1] << '\n';
                            }

                            string result = "Your order has been been cancel!\n";
                            send(sock, result.c_str(), result.length(), 0);
                        }
                    } else {
                        string result = "Some thing error in server\n";
                        send(sock, result.c_str(), result.length(), 0);
                        cout << "[Error] Fail to open " << ofname << " in directory" << endl;
                        break;
                    }

                } else {
                    cout << "[Error] Cancel order are not correct" << endl;

                    string result = "Your cancel are not correct\n";
                    send(sock, result.c_str(), result.length(), 0);
                }
            }
            if (recv_data[0] == '5') {
                cout << "[Exit] Terminate program" << endl;
                break;
            }

        }
        close(sock);
    }
    close(lsock);

    return 0;
}
